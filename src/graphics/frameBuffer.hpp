#pragma once

#include "texture2D.hpp"

class FrameBuffer {
public: 
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void bind();
	void unbind();
	void bindTexture();

	void clear();

public:
	unsigned int m_frameBufferId;
	Texture2D m_texture;
private:
	int prevViewportSettings[4];
};