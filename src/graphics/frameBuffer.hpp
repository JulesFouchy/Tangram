#pragma once

#include "texture2D.hpp"

class FrameBuffer {
public: 
	FrameBuffer();
	FrameBuffer(int width, int height);
	void Initialize(int width, int height);
	~FrameBuffer();

	void bind();
	void unbind();

	void clear();

	inline Texture2D& getTexture() { return m_texture; }

private:
	unsigned int m_frameBufferId;
	Texture2D m_texture;

	int m_prevViewportSettings[4];
};