#include "drawingBoard.hpp"

#include "graphics/immediateDrawing.hpp"

DrawingBoard::DrawingBoard(float whRatio) : m_whRatio(whRatio) {

}

DrawingBoard::~DrawingBoard() {

}

void DrawingBoard::show() {
	showFrame();
}

void DrawingBoard::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::rectOutline(0.0f, 0.0f, 0.9f * m_whRatio, 0.9f, 0.01f);
}