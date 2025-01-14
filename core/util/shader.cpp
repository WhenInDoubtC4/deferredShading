/*
*	Author: Eric Winebrenner
*	Extended to support subroutines
*/

#include "shader.h"
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string lastKnownFilepath = "<INLINE>";

namespace Util 
{
	/// <summary>
	/// Loads shader source code from a file.
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	std::string loadShaderSourceFromFile(const std::string& filePath) {
		lastKnownFilepath = filePath;
		std::ifstream fstream(filePath);
		if (!fstream.is_open()) {
			printf("Failed to load file %s\n", filePath.c_str());
			return {};
		}
		std::stringstream buffer;

		//Add shader version directive
#ifdef EMSCRIPTEN
		buffer << "#version 300 es\n";
#else
		buffer << "#version 450\n";
#endif

		buffer << fstream.rdbuf();
		return buffer.str();
	}

	/// <summary>
	/// Creates and compiles a shader object of a given type
	/// </summary>
	/// <param name="shaderType">Expects GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.</param>
	/// <param name="sourceCode">GLSL source code for the shader stage</param>
	/// <returns></returns>
	static unsigned int createShader(GLenum shaderType, const char* sourceCode) {
		//Create a new vertex shader object
		unsigned int shader = glCreateShader(shaderType);
		//Supply the shader object with source code
		glShaderSource(shader, 1, &sourceCode, NULL);
		//Compile the shader object
		glCompileShader(shader);
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			//512 is an arbitrary length, but should be plenty of characters for our error message.
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			printf("Failed to compile shader %s: %s\n", lastKnownFilepath.c_str(), infoLog);
		}
		return shader;
	}

	/// <summary>
	/// Creates a shader program with a vertex and fragment shader
	/// </summary>
	/// <param name="vertexShaderSource">GLSL source code for the vertex shader</param>
	/// <param name="fragmentShaderSource">GLSL source code for the fragment shader</param>
	/// <returns></returns>
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
		unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		unsigned int shaderProgram = glCreateProgram();
		//Attach each stage
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		//Link all the stages together
		glLinkProgram(shaderProgram);
		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			printf("Failed to link shader program %s: %s", lastKnownFilepath.c_str(), infoLog);
		}
		//The linked program now contains our compiled code, so we can delete these intermediate objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return shaderProgram;
	}
	/// <summary>
	/// Creates a shader instance with vertex + fragment stages
	/// </summary>
	/// <param name="vertexShader">File path to vertex shader</param>
	/// <param name="fragmentShader">File path to fragment shader</param>
	Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		std::string vertexShaderSource = Util::loadShaderSourceFromFile(vertexShader.c_str());
		std::string fragmentShaderSource = Util::loadShaderSourceFromFile(fragmentShader.c_str());
		m_id = Util::createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
	}

	void Shader::use()const
	{
		glUseProgram(m_id);
	}

	void Shader::setInt(const std::string& name, int v) const
	{
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), v);
	}

	void Shader::setFloat(const std::string& name, float v) const
	{
		glUniform1f(glGetUniformLocation(m_id, name.c_str()), v);
	}

	void Shader::setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
	}

	void Shader::setVec2(const std::string& name, const glm::vec2& v) const
	{
		setVec2(name, v.x, v.y);
	}

	void Shader::setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& v) const
	{
		setVec3(name, v.x, v.y, v.z);
	}

	void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
	}

	void Shader::setVec4(const std::string& name, const glm::vec4& v) const
	{
		setVec4(name, v.x, v.y, v.z, v.w);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& m) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
	}

	void Shader::setUniformBlock(const std::string& name, int value) const
	{
		glUniformBlockBinding(m_id, glGetUniformBlockIndex(m_id, name.c_str()), value);
	}

#ifndef EMSCRIPTEN
	void Shader::setSubroutine(GLenum shaderType, std::initializer_list<std::pair<std::string, std::string>> nameSelectionList)
	{
		//From https://www.informit.com/articles/article.aspx?p=2731929&seqNum=6
		//and https://www.geeks3d.com/20140701/opengl-4-shader-subroutines-introduction-3d-programming-tutorial/

		std::vector<std::pair<GLint, GLuint>> selections;

		for (const std::pair<std::string, std::string>& nameSelection : nameSelectionList)
		{
			GLint uniformLocation = glGetSubroutineUniformLocation(m_id, shaderType, nameSelection.first.c_str());

			if (uniformLocation < 0)
			{
				printf("Can't find subroutine %s\n", nameSelection.first.c_str());
				return;
			}

			GLuint selectionIndex = glGetSubroutineIndex(m_id, shaderType, nameSelection.second.c_str());
			if (selectionIndex == GL_INVALID_INDEX)
			{
				printf("Invalid subroutine selection %s\n", nameSelection.second.c_str());
				return;
			}

			selections.push_back(std::make_pair(uniformLocation, selectionIndex));
		}

		auto indices = new GLuint[selections.size()];
		for (const std::pair<GLint, GLuint>& selection : selections)
		{
			indices[selection.first] = selection.second;
		}

		glUniformSubroutinesuiv(shaderType, selections.size(), indices);

		delete[] indices;
	}
#endif

}

