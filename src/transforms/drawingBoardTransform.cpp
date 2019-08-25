#include "drawingBoardTransform.hpp"

#include "UI/input.hpp"

#include "spdlog/spdlog.h"

DrawingBoardTransform::DrawingBoardTransform(float aspectRatio)
	: RectTransform(aspectRatio), m_zoomInFactor(0.8f)
{
}


void DrawingBoardTransform::zoomIn() {
	scale(m_zoomInFactor);
}
void DrawingBoardTransform::zoomIn(glm::vec2 origin) {
	scale(m_zoomInFactor, origin);
}
void DrawingBoardTransform::zoomOut() {
	scale(1.0f / m_zoomInFactor);
}
void DrawingBoardTransform::zoomOut(glm::vec2 origin) {
	scale(1.0f / m_zoomInFactor, origin);
}

void DrawingBoardTransform::checkInputs() {
	checkDraggingTranslation();
	//Center the point we double-clic on
	if (Input::bDoubleLeftClic) {
		translate(-Input::getMousePosition());
	}
}

bool DrawingBoardTransform::onLeftClicDown() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown()) {
		startDraggingTranslation();
		return true;
	}
	return false;
}
bool DrawingBoardTransform::onSpaceBarDown() {
	//Moving by holding space + clic'n'dragging
	if (Input::leftClicIsDown()) {
		startDraggingTranslation();
		return true;
	}
	return false;
}
bool DrawingBoardTransform::onLeftClicUp() {
	//Moving by holding space + clic'n'dragging
	bool handled = bDraggingTranslation;
	endDraggingTranslation();
	return handled;
}
bool DrawingBoardTransform::onSpaceBarUp() {
	//Moving by holding space + clic'n'dragging
	bool handled = bDraggingTranslation;
	endDraggingTranslation();
	return handled;
}