#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "layer.hpp"
#include "layerList.hpp"

#include "graphics/frameBuffer.hpp"

#include "transforms/drawingBoardTransform.hpp"

#include "UI/cursor.hpp"

class DrawingBoard {
public:
	static void Initialize(float whRatio);

	static void show();
	static void showForSaving();
	static void save(int approxNbPixels, std::string filePath);

	static void addLayer(const std::string imgFilePath);

	inline static LayerList& getLayerList() { return layers; };

	static void update();

	static void onDoubleLeftClic();
	static void onLeftClicDown();
	static void onLeftClicUp();
	static void onSpaceBarDown();
	static void onSpaceBarUp();
	static void setCursor();

public:
	static glm::vec4 backgroundColor;

private:
	static bool m_bIsHandlingAnInput;
	static inline bool isHandlingAnInput() { return m_bIsHandlingAnInput; };
	static CursorType* m_currentCursor;
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