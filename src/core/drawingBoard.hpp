#pragma once

#include "glm/glm.hpp"

class DrawingBoard {
public:
	DrawingBoard(float whRatio);
	~DrawingBoard();
	void show();

	void checkInputs();
	void onLeftClicUp();
	void onSpaceBarUp();

	void zoomIm();
	void zoomOut();

private:
	void showFrame();
private:
	float m_whRatio;
	float m_scale;
	glm::vec2 m_translate;
	glm::vec2 m_prevTranslate;
	float m_zoomInFactor;
};