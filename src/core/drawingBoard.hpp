#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "layer.hpp"

class DrawingBoard {
public:
	DrawingBoard(float whRatio);
	~DrawingBoard();
	void show();

	void addLayer(std::string imgFilePath);

	void checkInputs();
	void onLeftClicUp();
	void onSpaceBarUp();

	void zoomIn();
	void zoomOut();

private:
	void showFrame();
private:
	std::vector<Layer*> layers;
private:
	float m_whRatio;
	float m_scale;
	glm::vec2 m_translate;
	glm::vec2 m_prevTranslate;
	float m_zoomInFactor;
};