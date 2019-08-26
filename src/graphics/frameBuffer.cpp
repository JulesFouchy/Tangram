#include "frameBuffer.hpp"

#include <iostream>

#include <GL/glew.h>

#include "utilities/display.hpp"

FrameBuffer::FrameBuffer(int width, int height)
{
	//Gen Buffer
	glGenFramebuffers(1, &m_frameBufferId);
	//
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	//Gen texture
	m_texture.Initialize(width, height, 4);
	//Attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.m_textureID, 0);
	//Check for completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &m_frameBufferId);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	glGetIntegerv(GL_VIEWPORT, prevViewportSettings);
	glViewport(0, 0, m_texture.m_width, m_texture.m_height);
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(prevViewportSettings[0], prevViewportSettings[1], prevViewportSettings[2], prevViewportSettings[3]);
}

void FrameBuffer::bindTexture() {
	m_texture.bind();
}

void FrameBuffer::clear() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}