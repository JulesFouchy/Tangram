#include "drawingBoard.hpp"

#include "graphics/immediateDrawing.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "UI/input.hpp"
#include "utilities/display.hpp"

#include "spdlog/spdlog.h"


#include "stb_image/stb_image_write.h"

DrawingBoard::DrawingBoard(float whRatio) 
	: m_whRatio(whRatio), m_translation(glm::vec2(0.0f)), m_prevTranslation(glm::vec2(0.0f)), m_scale(0.9f), m_zoomInFactor(0.8f),
	m_rotation(0.0f), m_transform(glm::mat4x4(1.0f))
{

}

DrawingBoard::~DrawingBoard() {

}

void DrawingBoard::show() {
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->show(m_transform);
	}
	showFrame();
}

void DrawingBoard::showForSaving() {
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->show(m_transform, glm::ortho(-0.5f * m_whRatio, 0.5f * m_whRatio, -0.5f, 0.5f));
	}
}

void DrawingBoard::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * m_transform);
	ImmediateDrawing::rectOutline(0.0f, 0.0f, m_whRatio, 1.0f, 0.002f);
}

void DrawingBoard::save(int approxNbPixels, std::string filePath) {
	//Compute output width and height
	float w = sqrt(approxNbPixels * m_whRatio);
	float h = w / m_whRatio;
	int width = floor(w);
	int height = floor(h);
	//Bind frameBuffer to render and save
	FrameBuffer saveBuffer(width, height);
	saveBuffer.bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	resetTransform();
	glViewport(0, 0, width, height);
	showForSaving();
	//
	unsigned char* data = new unsigned char[4*width*height];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(1);
	stbi_write_png(filePath.c_str(), width, height, 4, data, 0);
	//
	saveBuffer.unbind();
}

void DrawingBoard::setTranslation(glm::vec2 translation) {
	m_translation = translation;
	computeTransformMatrix();
}
void DrawingBoard::translate(glm::vec2 translation) {
	setTranslation(m_translation + translation);
}
void DrawingBoard::setScale(float scale) {
	m_scale = scale;
	computeTransformMatrix();
}
void DrawingBoard::scale(float scale) {
	setScale(m_scale * scale);
}
void DrawingBoard::setRotation(float rotation) {
	m_rotation = rotation;
	computeTransformMatrix();
}
void DrawingBoard::rotate(float rotation) {
	setRotation(m_rotation + rotation);
}
void DrawingBoard::computeTransformMatrix() {
	m_transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(m_translation, 0.0f));
	m_transform = glm::rotate(m_transform, m_rotation, glm::vec3(0.0, 0.0, 1.0));
	m_transform = glm::scale(m_transform, glm::vec3(m_scale, m_scale, 1.0f));
}
void DrawingBoard::resetTransform() {
	setTranslation(glm::vec2(0.0f));
	setScale(1.0f);
	setRotation(0.0f);
}

void DrawingBoard::addLayer(std::string imgFilePath) {
	layers.push_back(new Layer(imgFilePath));
}

void DrawingBoard::checkInputs() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown() && Input::leftClicIsDown()) {
		m_translation = m_prevTranslation + Input::getMousePosition() - Input::getMousePosWhenLeftClicAndSpaceBarDown();
		computeTransformMatrix();
	}
}

void DrawingBoard::onLeftClicUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown()) {
		m_prevTranslation += Input::getMousePosition() - Input::getMousePosWhenLeftClicAndSpaceBarDown();
		m_translation = m_prevTranslation;
		computeTransformMatrix();
	}
}

void DrawingBoard::onSpaceBarUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::leftClicIsDown()) {
		m_prevTranslation += Input::getMousePosition() - Input::getMousePosWhenLeftClicAndSpaceBarDown();
		m_translation = m_prevTranslation;
		computeTransformMatrix();
	}
}

void DrawingBoard::zoomIn() {
	scale(m_zoomInFactor);
}

void DrawingBoard::zoomOut() {
	scale(1.0f/m_zoomInFactor);
}