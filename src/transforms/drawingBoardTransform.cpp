#include "drawingBoardTransform.hpp"

#include "UI/input.hpp"

#include "UI/log.hpp"

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

void DrawingBoardTransform::checkDraggingTranslation(){
	//Override the base function because in that specific case we musn't take into account the drawingBoard view matrix
	if (bDraggingTranslation) {
		glm::vec4 dl = glm::vec4(Input::getMousePosition() - m_mousePosWhenDraggingStarted, 0.0f, 0.0f);
		setTranslation(m_translationWhenDraggingStarted + glm::vec2(dl.x, dl.y));
	}
}