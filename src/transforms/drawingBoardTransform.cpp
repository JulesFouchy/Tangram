#include "drawingBoardTransform.hpp"

#include "UI/input.hpp"

#include "UI/log.hpp"

#include "UI/settings.hpp"

DrawingBoardTransform::DrawingBoardTransform(Ratio aspectRatio)
	: RectTransform(aspectRatio)
{
}

void DrawingBoardTransform::zoomIn() {
	scale(Settings::ZOOM_FACTOR);
}
void DrawingBoardTransform::zoomIn(glm::vec2 origin) {
	scale(Settings::ZOOM_FACTOR, origin);
}
void DrawingBoardTransform::zoomOut() {
	scale(1.0f / Settings::ZOOM_FACTOR);
}
void DrawingBoardTransform::zoomOut(glm::vec2 origin) {
	scale(1.0f / Settings::ZOOM_FACTOR, origin);
}

void DrawingBoardTransform::checkDraggingTranslation(){
	//Override the base function because in that specific case we musn't take into account the drawingBoard view matrix
	if (bDraggingTranslation) {
		glm::vec4 dl = glm::vec4(Input::getMousePosition() - m_mousePosWhenDraggingStarted, 0.0f, 0.0f);
		setTranslation(m_translationWhenDraggingStarted + glm::vec2(dl.x, dl.y));
	}
}