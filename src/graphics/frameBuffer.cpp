#include "frameBuffer.hpp"

#include <iostream>

#include <GL/glew.h>

#include "utilities/display.hpp"

FrameBuffer::FrameBuffer(int width, int height) {
	//Gen framebuffer
	glGenFramebuffers(1, &m_frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	//Gen texture
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);
	//Check for completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &m_frameBufferId);
	glDeleteTextures(1, &m_textureId);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}