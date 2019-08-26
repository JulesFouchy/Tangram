#pragma once

#include "layer.hpp"

#include <vector>

enum MousePositionRelativeToLayer {
	OUTSIDE,
	INSIDE,
	RIGHT,
	TOP_RIGHT,
	TOP,
	TOP_LEFT,
	LEFT,
	BOT_LEFT,
	BOT,
	BOT_RIGHT
};

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
private:
	void computeHoveredLayerAndMouseRelPos();
private:
	int m_activLayerIndex;
	Layer* hoveredLayer;
	MousePositionRelativeToLayer mousePosRelToHoveredLayer;
};