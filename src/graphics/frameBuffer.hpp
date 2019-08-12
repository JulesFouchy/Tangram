#pragma once

#include "graphics/shader.hpp"

class FrameBuffer {
public: 
	FrameBuffer(Shader& shader);
	~FrameBuffer();

	void bind();
	void unbind();

	void renderToScreen();

private:
	unsigned int m_frameBufferId;
	unsigned int m_textureId;
	unsigned int m_vertexBufferId;
	unsigned int m_indexBufferId;

	Shader& m_shader;
};