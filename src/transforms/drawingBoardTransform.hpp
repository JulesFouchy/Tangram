#pragma once

#include "rectTransform.hpp"

#include "utilities/ratio.hpp"

class DrawingBoardTransform : public RectTransform {
public:
	DrawingBoardTransform(Ratio aspectRatio);

	void zoomIn();
	void zoomIn(glm::vec2 origin);
	void zoomOut();
	void zoomOut(glm::vec2 origin);

	void checkDraggingTranslation();
};