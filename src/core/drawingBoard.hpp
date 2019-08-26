#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "layer.hpp"

#include "graphics/frameBuffer.hpp"

#include "transforms/drawingBoardTransform.hpp"

class Layer;

class DrawingBoard {
public:
	DrawingBoard(float whRatio);
	~DrawingBoard();

	void show();
	void showForSaving();
	void save(int approxNbPixels, std::string filePath);

	void addLayer(std::string imgFilePath);

	Layer* getActivLayer();
	void setActivLayer(int layerIndex);

	void onLeftClicDown();
	void onLeftClicUp();

public:
	DrawingBoardTransform transform;
private:
	int m_activLayerIndex;
private:
	void showFrame();
public:
	std::vector<Layer*> layers;
private:
	FrameBuffer renderBuffer;
};