#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "layer.hpp"

#include "graphics/frameBuffer.hpp"

class DrawingBoard {
public:
	DrawingBoard(float whRatio);
	~DrawingBoard();

	void show();
	void showForSaving();
	void save(int approxNbPixels, std::string filePath);

	void addLayer(std::string imgFilePath);

	void checkInputs();
	void onLeftClicUp();
	void onSpaceBarUp();

	void setTranslation(glm::vec2 translation);
	void translate(glm::vec2 translation);
	void setScale(float scale);
	void scale(float scale);
	void setRotation(float rotation);
	void rotate(float rotation);
	void zoomIn();
	void zoomOut();
	void resetTransform();

private:
	void showFrame();
private:
	std::vector<Layer*> layers;
private:
	float m_whRatio;
	glm::vec2 m_translation;
	glm::vec2 m_prevTranslation;
	float m_scale;
	float m_zoomInFactor;
	float m_rotation;
	glm::mat4x4 m_transform;
	void computeTransformMatrix();
};