#include "frameBuffer.hpp"

#include <iostream>

#include <GL/glew.h>

#include "UI/log.hpp"

#include "helper/display.hpp"

FrameBuffer::FrameBuffer()
	: m_frameBufferId(0), m_texture(), m_prevViewportSettings()
{
	// Gen Buffer
	glGenFramebuffers(1, &m_frameBufferId);
	// Bind
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	// Attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.getID(), 0);
	// Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(int width, int height)
	: FrameBuffer()
{
	setTextureSize(width, height);
}

void FrameBuffer::setTextureSize(int width, int height) {
	m_texture.Initialize(width, height, 4);
	// Check for completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		spdlog::error("Framebuffer is not complete!");
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &m_frameBufferId);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	glGetIntegerv(GL_VIEWPORT, m_prevViewportSettings); // Store viewport settings to restore them when unbinding
	glViewport(0, 0, m_texture.getWidth(), m_texture.getHeight());
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_prevViewportSettings[0], m_prevViewportSettings[1], m_prevViewportSettings[2], m_prevViewportSettings[3]);
}

void FrameBuffer::clear() {
	// Make sure you have bound the framebuffer before calling clear()
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}