#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "layer/layer.hpp"
#include "registryOfAllLayersPresentAndGone.hpp"
#include "history/history.hpp"

#include "graphics/frameBuffer.hpp"

#include "transforms/drawingBoardTransform.hpp"

#include "helper/ratio.hpp"

#include "UI/cursor.hpp"

#include "UI/keys.hpp"

class DrawingBoard {
public:
	static void Initialize(Ratio aspectRatio);

	static void show();
	static void save(unsigned int height, const std::string& filePath);

	static void update();

	static inline RegistryOfAllLayersPresentAndGone& LayerRegistry() { return m_LayerRegistry; }

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
private:
	static RegistryOfAllLayersPresentAndGone m_LayerRegistry;
private:
	static FrameBuffer* renderBuffer;
private:
	DrawingBoard() = default;
	~DrawingBoard() = default;
};