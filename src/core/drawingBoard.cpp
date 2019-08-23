#include "drawingBoard.hpp"

#include "graphics/immediateDrawing.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "UI/input.hpp"
#include "utilities/display.hpp"

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

void DrawingBoard::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(glm::ortho(Display::getMinX(), Display::getMaxX(), Display::getMinY(), Display::getMaxY()) * m_transform);
	ImmediateDrawing::rectOutline(0.0f, 0.0f, m_whRatio, 1.0f, 0.002f);
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

void DrawingBoard::addLayer(std::string imgFilePath) {
	layers.push_back(new Layer(imgFilePath));
}

void DrawingBoard::checkInputs() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown() && Input::leftClicIsDown()) {
		m_translation = m_prevTranslation + Input::getMousePosition() - Input::getMousePosWhenLeftClicDown();
		computeTransformMatrix();
	}
}

void DrawingBoard::onLeftClicUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown()) {
		m_prevTranslation += Input::getMousePosition() - Input::getMousePosWhenLeftClicDown();
		m_translation = m_prevTranslation;
		computeTransformMatrix();
	}
}

void DrawingBoard::onSpaceBarUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::leftClicIsDown()) {
		m_prevTranslation += Input::getMousePosition() - Input::getMousePosWhenLeftClicDown();
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