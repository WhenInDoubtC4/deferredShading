#include "framebuffer.h"

#include <cstdint>

constexpr int MAX_COLOR_ATTACHMENTS = 8;

//Why :(
constexpr GLenum COLOR_ATTACHMENTS[MAX_COLOR_ATTACHMENTS] = {
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7,
};

namespace Util
{
	Framebuffer::Framebuffer(const glm::vec2& size)
		: _size(size)
	{
		//Delete previous attachments
		_colorAttachments.clear();

		glCreateFramebuffers(1, &_fbo);
	}

	GLuint Framebuffer::addColorAttachment(GLenum colorFormat, GLint wrapS, GLint wrapT, GLint magFilter, GLint minFilter)
	{
		if (_colorAttachments.size() >= MAX_COLOR_ATTACHMENTS)
		{
			printf("Framebuffer%i exceeds maximum number of color attachments\n", _fbo);
			return -1;
		}

		GLuint colorAttachment;

		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		glGenTextures(1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, colorFormat, _size.x, _size.y);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glFramebufferTexture(GL_FRAMEBUFFER, COLOR_ATTACHMENTS[_colorAttachments.size()], colorAttachment, 0);

		_colorAttachments.push_back(colorAttachment);

		//Tell OpenGL which attachments to draw to
		setGLDrawBuffers();

		return colorAttachment;
	}

	GLuint Framebuffer::addDepthAttachment()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		glGenTextures(1, &_depthAttachment);
		glBindTexture(GL_TEXTURE_2D, _depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, _size.x, _size.y);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthAttachment, 0);

		//Tell OpenGL which attachments to draw to
		setGLDrawBuffers();

		return _depthAttachment;
	}

	bool Framebuffer::isComplete() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	GLuint Framebuffer::getFBO() const
	{
		return _fbo;
	}

	GLuint Framebuffer::getColorAttachment(int index) const
	{
		return _colorAttachments[index];
	}

	void Framebuffer::setGLDrawBuffers() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		std::vector<GLenum> drawBuffers;
		for (int i = 0; i < _colorAttachments.size(); i++)
		{
			drawBuffers.push_back(COLOR_ATTACHMENTS[i]);
		}
		if (_depthAttachment != 0) drawBuffers.push_back(GL_DEPTH_ATTACHMENT);

		//*scream*
		uint32_t s = drawBuffers.size() - 1;
		glDrawBuffers(s, drawBuffers.data());
	}
}