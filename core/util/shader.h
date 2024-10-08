/*
*	Author: Eric Winebrenner
*	Extended to support subroutines
*/

#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../ew/external/glad.h"

namespace Util 
{
	std::string loadShaderSourceFromFile(const std::string& filePath);
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
	
	class Shader 
	{
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		GLuint getid() const { return m_id; };
		void use() const;
		void setInt(const std::string& name, int v) const;
		void setFloat(const std::string& name, float v) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec2(const std::string& name, const glm::vec2& v) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec3(const std::string& name, const glm::vec3& v) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
		void setVec4(const std::string& name, const glm::vec4& v) const;
		void setMat4(const std::string& name, const glm::mat4& m) const;
		void setSubroutine(GLenum shaderType, std::initializer_list<std::pair<std::string, std::string>> nameSelectionList);

	private:
		unsigned int m_id; //Shader program handle
	};
}