#pragma once

#include "glm/glm.hpp"

#include "graphics/loadedImage.hpp"
#include "drawingBoard.hpp"
#include "transforms/rectTransform.hpp"

class DrawingBoard;

class Layer {
public:
	Layer(std::string imgFilePath, DrawingBoard* drawingBoard, std::string layerName = "New layer");
	~Layer();

	void show();
	void showFrame();

	void checkInputs();

private:
	LoadedImage m_image;
	DrawingBoard* m_drawingBoard;
	std::string m_name;
public:
	RectTransform m_transform;
};