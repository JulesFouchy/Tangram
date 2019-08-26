#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "layer.hpp"
#include "layerList.hpp"

#include "graphics/frameBuffer.hpp"

#include "transforms/drawingBoardTransform.hpp"

class DrawingBoard {
public:
	DrawingBoard(float whRatio);
	~DrawingBoard();

	void show();
	void showForSaving();
	void save(int approxNbPixels, std::string filePath);

	void addLayer(const std::string imgFilePath);

	void onLeftClicDown();
	void onLeftClicUp();

public:
	DrawingBoardTransform transform;
private:
	void showFrame();
public:
	LayerList layers;
private:
	FrameBuffer renderBuffer;
};