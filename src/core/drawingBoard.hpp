#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "layer/layer.hpp"
#include "layerList.hpp"
#include "history/history.hpp"

#include "graphics/frameBuffer.hpp"

#include "transforms/drawingBoardTransform.hpp"

#include "utilities/ratio.hpp"

#include "UI/cursor.hpp"

#include "UI/keys.hpp"

class DrawingBoard {
public:
	static void Initialize(Ratio aspectRatio);

	static void show();
	static void showForSaving();
	static void save(int approxNbPixels, std::string filePath);

	inline static LayerList& getLayerList() { return layers; };

	static void update();

	static void onDoubleLeftClic();
	static void onLeftClicDown();
	static void onLeftClicUp();
	static void onScroll(float motion);
	static void onKeyDown(Key key);
	static void onKeyUp(Key key);
	static void setCursor();

public:
	static glm::vec4 backgroundColor;
	static glm::vec4 outsideColor;

private:
	static bool m_bIsHandlingAnInput;
	static inline bool isHandlingAnInput() { return m_bIsHandlingAnInput; };
	static CursorType* m_currentCursor;
public:
	static History history;
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