#pragma once

#include "layer.hpp"

#include <vector>

class LayerList {
public:
	LayerList();
	~LayerList();

	void update();

	void show();
	void showFrames();

	void addLayer(std::string imgFilePath);
	Layer* getActivLayer();
	void setActivLayer(int layerIndex);

	void onLeftClicDown();
	void onLeftClicUp();

	inline bool isHandlingAnInput() { return m_isHandlingAnInput; };

public:
	std::vector<Layer*> layers;
private:
	bool m_isHandlingAnInput;
private:
	void computeHoveredLayerAndMouseRelPos();
public:
	Layer* m_activLayer;
	Layer* m_hoveredLayer;
	MousePositionRelativeToRect m_mousePosRelToHoveredLayer;
};