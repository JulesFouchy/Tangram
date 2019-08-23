#pragma once

#include "graphics/shader.hpp"

class FrameBuffer {
public: 
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void bind();
	void unbind();

private:
	unsigned int m_frameBufferId;
	unsigned int m_textureId;
};