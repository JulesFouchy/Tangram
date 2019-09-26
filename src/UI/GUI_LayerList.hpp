#pragma once

#include "core/layer/layer.hpp"

class GUI_LayerList {
public:
	static void show();
	static void dragDropSourceReorderLayer(Layer* layer);
	static void dragDropTargetReorderLayer(int layerIndex);
};