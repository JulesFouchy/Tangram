#include "drawingBoard.hpp"

#include "graphics/immediateDrawing.hpp"

#include "UI/input.hpp"

DrawingBoard::DrawingBoard(float whRatio) : m_whRatio(whRatio), m_scale(1.0f), m_translate(glm::vec2(0.0f)), m_prevTranslate(glm::vec2(0.0f)), m_zoomInFactor(0.8f){

}

DrawingBoard::~DrawingBoard() {

}

void DrawingBoard::show() {
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->show(m_translate, m_scale);
	}
	showFrame();
}

void DrawingBoard::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::rectOutline(m_translate.x, m_translate.y, m_scale * m_whRatio, m_scale, 0.002f);
}

void DrawingBoard::addLayer(std::string imgFilePath) {
	layers.push_back(new Layer(imgFilePath));
}

void DrawingBoard::checkInputs() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown() && Input::leftClicIsDown()) {
		m_translate = m_prevTranslate + Input::getMousePosition() - Input::getMousePosWhenLeftClicDown();
	}
}

void DrawingBoard::onLeftClicUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown()) {
		m_prevTranslate += Input::getMousePosition() - Input::getMousePosWhenLeftClicDown();
		m_translate = m_prevTranslate;
	}
}

void DrawingBoard::onSpaceBarUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::leftClicIsDown()) {
		m_prevTranslate += Input::getMousePosition() - Input::getMousePosWhenLeftClicDown();
		m_translate = m_prevTranslate;
	}
}

void DrawingBoard::zoomIn() {
	m_scale *= m_zoomInFactor;
}

void DrawingBoard::zoomOut() {
	m_scale /= m_zoomInFactor;
}