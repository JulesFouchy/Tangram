#pragma once

#include "texture2D.hpp"

class FrameBuffer {
public: 
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void bind();
	void unbind();

	void clear();

	inline const Texture2D& getTexture() { return m_texture; }

private:
	unsigned int m_frameBufferId;
	Texture2D m_texture;

	int m_prevViewportSettings[4];
};