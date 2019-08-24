#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "layer.hpp"

#include "graphics/frameBuffer.hpp"

#include "transforms/DrawingBoardTransform.hpp"

class DrawingBoard {
public:
	DrawingBoard(float whRatio);
	~DrawingBoard();

	void show();
	void showForSaving();
	void save(int approxNbPixels, std::string filePath);

	void addLayer(std::string imgFilePath);

public:
	ViewTransform transform;
private:
	void showFrame();
private:
	std::vector<Layer*> layers;
};