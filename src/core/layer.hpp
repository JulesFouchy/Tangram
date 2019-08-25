#pragma once

#include "glm/glm.hpp"

#include "image.hpp"
#include "drawingBoard.hpp"
#include "transforms/rectTransform.hpp"

class DrawingBoard;

class Layer {
public:
	Layer(std::string imgFilePath, DrawingBoard* drawingBoard, std::string layerName = "New layer");
	~Layer();

	void show(glm::mat4x4 projection);
	void show();

	void checkInputs();

private:
	Image m_image;
	DrawingBoard* m_drawingBoard;
	std::string m_name;
public:
	RectTransform m_transform;
};