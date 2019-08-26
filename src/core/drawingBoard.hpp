#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "layer.hpp"
#include "layerList.hpp"

#include "graphics/frameBuffer.hpp"

#include "transforms/drawingBoardTransform.hpp"

class DrawingBoard {
public:
	static void Initialize(float whRatio);

	static void show();
	static void showForSaving();
	static void save(int approxNbPixels, std::string filePath);

	static void addLayer(const std::string imgFilePath);

	static void onLeftClicDown();
	static void onLeftClicUp();

public:
	static DrawingBoardTransform transform;
private:
	static void showFrame();
public:
	static LayerList layers;
private:
	static FrameBuffer* renderBuffer;
private:
	DrawingBoard() = default;
	~DrawingBoard() = default;
};