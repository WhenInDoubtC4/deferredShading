#pragma once

#include <vector>
#include <glm/glm.hpp>

#ifdef EMSCRIPTEN
#include <GLFW/emscripten_glfw3.h>
#include <GLES3/gl3.h>
#else
#include "../ew/external/glad.h"
#endif

namespace Util
{
	class Framebuffer
	{
	public:
		Framebuffer() {};
		Framebuffer(const glm::vec2& size);

		GLuint addColorAttachment(GLenum colorFormat = GL_RGBA8, 
			GLint wrapS = GL_CLAMP_TO_EDGE,
			GLint wrapT = GL_CLAMP_TO_EDGE,
			GLint magFilter = GL_LINEAR,
			GLint minFilter = GL_LINEAR);
		GLuint addDepthAttachment();

		bool isComplete() const;
		glm::vec2 getSize() const { return _size; };
		GLuint getFBO() const;
		GLuint getColorAttachment(int index = 0) const;
		GLuint getDepthAttachment() const { return _depthAttachment; };
		size_t getNumColorAttachments() const { return _colorAttachments.size(); };
		void setGLDrawBuffers() const;

	private:
		glm::vec2 _size;
		GLuint _fbo;
		std::vector<GLuint> _colorAttachments;
		GLuint _depthAttachment;
	};
}