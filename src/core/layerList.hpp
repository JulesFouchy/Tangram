#pragma once

#include "layer.hpp"

#include <vector>

#include "UI/cursor.hpp"

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

	void onDoubleLeftClic();
	void onLeftClicDown();
	void onLeftClicUp();
	void setCursor();

	inline bool isHandlingAnInput() { return m_bIsHandlingAnInput; };

private:
	bool mustShowAltOrigin();
	bool mouseIsHoveringAltOrigin();
	bool canDragRotation();
public:
	std::vector<Layer*> layers;
private:
	bool m_bIsHandlingAnInput;
	CursorStyle* usedCursor;
private:
	void computeHoveredLayerAndMouseRelPos();
public:
	Layer* m_activLayer;
	Layer* m_hoveredLayer;
	MousePositionRelativeToRect m_mousePosRelToHoveredLayer;
};