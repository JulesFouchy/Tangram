#pragma once

#include "rectTransform.hpp"

class DrawingBoardTransform : public RectTransform {
public:
	DrawingBoardTransform(float aspectRatio);

	void zoomIn();
	void zoomIn(glm::vec2 origin);
	void zoomOut();
	void zoomOut(glm::vec2 origin);

	void checkDraggingTranslation();

private:
	float m_zoomInFactor;
};