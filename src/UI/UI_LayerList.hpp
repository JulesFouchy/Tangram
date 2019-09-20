#pragma once

#include "core/layer/layer.hpp"

class UI_LayerList {
public:
	static void show();
	static void dragDropSourceReorderLayer(Layer* layer);
	static void dragDropTargetReorderLayer(int layerIndex);
};