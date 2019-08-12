#include "renderer.hpp"

Renderer::Renderer()
{
	//Gen vertex buffer for full quad
	float vertices[] = {
		//Position
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f,
		1.0f, -1.0f,
	};
	glGenBuffers(1, &m_fullQuadVBid);
	glBindBuffer(GL_ARRAY_BUFFER, m_fullQuadVBid);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Gen index buffer
	unsigned int indices[] = {
		0, 2, 1,
		0, 3, 2
	};
	glGenBuffers(1, &m_fullQuadIBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullQuadIBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Renderer::~Renderer() {

}

void Renderer::drawFullQuad() {
	glBindBuffer(GL_ARRAY_BUFFER, m_fullQuadVBid);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullQuadIBOid);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}