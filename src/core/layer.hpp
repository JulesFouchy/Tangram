#pragma once

#include "glm/glm.hpp"

#include "graphics/loadedImage.hpp"
#include "drawingBoard.hpp"
#include "transforms/rectTransform.hpp"

class DrawingBoard;

class Layer {
public:
	float mX;
	float MX;
	float mY;
	float MY;
public:
	Layer(std::string imgFilePath, DrawingBoard* drawingBoard, std::string layerName = "New layer");
	~Layer();

	void show(glm::mat4x4 projection);
	void show();
	void showFrame(const glm::mat4x4& viewMatrix);

	void checkInputs();

private:
	LoadedImage m_image;
	DrawingBoard* m_drawingBoard;
	std::string m_name;
public:
	RectTransform m_transform;
};