#include "immediateDrawing.hpp"

#include "helper/display.hpp"
#include "helper/maths.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

unsigned int ImmediateDrawing::m_fullQuadVBid;
unsigned int ImmediateDrawing::m_fullQuadIBOid;
glm::mat4x4 ImmediateDrawing::m_viewProjMatrix;
glm::vec4 ImmediateDrawing::m_color(0.0f, 0.0f, 0.0f, 1.0f);
Shader ImmediateDrawing::m_flatColorShader("res/shaders/vertex/standard.vert", "res/shaders/fragment/flatColor.frag", false);
Shader ImmediateDrawing::m_ringShader("res/shaders/vertex/texture.vert", "res/shaders/fragment/ring.frag", false);
Shader ImmediateDrawing::m_windowMinusARectangleShader("res/shaders/vertex/texture.vert", "res/shaders/fragment/windowMinusARectangle.frag", false);

void ImmediateDrawing::genBuffers() {
	//Gen vertex buffer for full quad
	float vertices[] = {
		//Position                              //TexCoord
		Display::getMinX(), Display::getMinY(), 0.0f, 0.0f,
		Display::getMinX(), Display::getMaxY(), 0.0f, 1.0f,
		Display::getMaxX(), Display::getMaxY(), 1.0f, 1.0f,
		Display::getMaxX(), Display::getMinY(), 1.0f, 0.0f
	};
	glGenBuffers(1, &m_fullQuadVBid);
	glBindBuffer(GL_ARRAY_BUFFER, m_fullQuadVBid);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);
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

void ImmediateDrawing::Initialize()
{
	//View matrix
	m_viewProjMatrix = glm::mat4x4(1.0f);
	//Compile shader
	m_flatColorShader.compile();
	m_ringShader.compile();
	m_windowMinusARectangleShader.compile();
	//
	genBuffers();
}

ImmediateDrawing::~ImmediateDrawing() {

}

void ImmediateDrawing::setViewProjMatrix(glm::mat4x4 viewProjMatrix) {
	m_viewProjMatrix = viewProjMatrix;
}

void ImmediateDrawing::setColor(float r, float g, float b, float a) {
	m_color = glm::vec4(r, g, b, a);
}

void ImmediateDrawing::shaderlessFullscreenRect() {
	glBindBuffer(GL_ARRAY_BUFFER, m_fullQuadVBid);
	//pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	//texCoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));
	//IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullQuadIBOid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void ImmediateDrawing::rect(float x, float y, float w, float h){
	glm::mat4x4 modelMatrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(x, y, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(w/Display::getRatio(), h, 1.0f));
	glm::mat4x4 mvp = m_viewProjMatrix * modelMatrix;
	m_flatColorShader.bind();
	m_flatColorShader.setUniformMat4f("u_mvp", mvp);
	m_flatColorShader.setUniform4f("u_color", m_color);

	shaderlessFullscreenRect();
}

void ImmediateDrawing::rectOutline(float x, float y, float w, float h, float thickness) {
	rect(x-w/2-thickness/2, y, thickness, h);
	rect(x+w/2+thickness/ 2, y, thickness, h);
	rect(x , y-h/2-thickness/2, w, thickness);
	rect(x, y+h/2+thickness/2, w, thickness);
}

void ImmediateDrawing::ring(float x, float y, float innerR, float outerR) {
	glm::mat4x4 modelMatrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(x, y, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(outerR * 2.0f / Display::getRatio(), outerR * 2.0f, 1.0f));
	glm::mat4x4 mvp = m_viewProjMatrix * modelMatrix;
	m_ringShader.bind();
	m_ringShader.setUniformMat4f("u_mvp", mvp);
	m_ringShader.setUniform4f("u_color", m_color);
	m_ringShader.setUniform1f("u_innerRadius", innerR/ outerR);

	shaderlessFullscreenRect();
}

void ImmediateDrawing::windowMinusARectangle(glm::vec2 centerInFragSpace, float w, float h, float angle) {
	m_windowMinusARectangleShader.bind();
	glm::mat4x4 mvp = Display::getProjMat();
	m_windowMinusARectangleShader.setUniformMat4f("u_mvp", mvp);
	m_windowMinusARectangleShader.setUniform4f("u_color", m_color);

	glm::vec2 wAxis = Maths::rotate(glm::vec2(2.0f/w, 0.0f), angle);
	glm::vec2 hAxis = Maths::rotate(glm::vec2(0.0f, 2.0f/h), angle);

	m_windowMinusARectangleShader.setUniform2f("u_centerInFragSpace", centerInFragSpace);
	m_windowMinusARectangleShader.setUniform2f("u_wAxis", wAxis);
	m_windowMinusARectangleShader.setUniform1f("u_width", w);
	m_windowMinusARectangleShader.setUniform2f("u_hAxis", hAxis);
	m_windowMinusARectangleShader.setUniform1f("u_height", h);

	shaderlessFullscreenRect();
}