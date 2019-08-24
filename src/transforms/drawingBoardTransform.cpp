#include "DrawingBoardTransform.hpp"

#include "UI/input.hpp"

#include "spdlog/spdlog.h"

ViewTransform::ViewTransform(float aspectRatio)
	: RectTransform(aspectRatio), m_zoomInFactor(0.8f)
{
	setScale(0.9f);
}


void ViewTransform::zoomIn() {
	scale(m_zoomInFactor);
}
void ViewTransform::zoomIn(glm::vec2 origin) {
	scale(m_zoomInFactor, origin);
}
void ViewTransform::zoomOut() {
	scale(1.0f / m_zoomInFactor);
}
void ViewTransform::zoomOut(glm::vec2 origin) {
	scale(1.0f / m_zoomInFactor, origin);
}

void ViewTransform::checkInputs() {
	checkDraggingTranslation();
	//Center the point we double-clic on
	if (Input::bDoubleLeftClic) {
		translate(-Input::getMousePosition());
	}
}

void ViewTransform::onLeftClicDown() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown() && !bDraggingTranslation) {
		startDraggingTranslation();
	}
}
void ViewTransform::onSpaceBarDown() {
	//Moving by holding space + clic'n'dragging
	if (Input::leftClicIsDown() && !bDraggingTranslation) {
		startDraggingTranslation();
	}
}
void ViewTransform::onLeftClicUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown()) {
		endDraggingTranslation();
	}
}
void ViewTransform::onSpaceBarUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::leftClicIsDown()) {
		endDraggingTranslation();
	}
}