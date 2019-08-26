#pragma once

#include "layer.hpp"

#include <vector>

class LayerList {
public:
	LayerList();
	~LayerList();

	void show();
	void showFrames();

	void addLayer(std::string imgFilePath);
	Layer* getActivLayer();
	void setActivLayer(int layerIndex);

public:
	std::vector<Layer*> layers;
public:
	void computeHoveredLayerAndMouseRelPos();
public:
	int m_activLayerIndex;
	Layer* m_hoveredLayer;
	MousePositionRelativeToRect m_mousePosRelToHoveredLayer;
};