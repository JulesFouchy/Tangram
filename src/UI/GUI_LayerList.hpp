#pragma once

#include "core/layer/layer.hpp"

#include "core/layerID.hpp"

class GUI_LayerList {
public:
	static void show();
	static void dragDropSourceReorderLayer(LayerID layerID);
	static void dragDropTargetReorderLayer(int layerIndex);
private:
	static void ImGuiShowTexture(Layer* layer, unsigned int previewHeight);
	static void ImGuiBeginLayerChildWindow(LayerID layerID, unsigned int height);
};