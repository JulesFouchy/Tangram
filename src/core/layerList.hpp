#pragma once

#include "layer/layer.hpp"

#include "groupOfLayers.hpp"

#include <vector>

#include "UI/cursor.hpp"
#include "UI/keys.hpp"

#include "transforms/draggablePoint.hpp"

#include "layerID.hpp"

class LayerList {
public:
	LayerList();
	~LayerList();

	void update();

	void show(glm::mat4x4 viewMatrix, glm::mat4x4 projMatrix);
	void showForSaving();
	void showSelectedLayerGUI();

	//Layer* getLayer(int index);
	void setSelectedLayer(LayerID layerID);

	void onKeyDown(Key key);
	void onKeyUp(Key key);
	void onDoubleLeftClic();
	void onLeftClicDown();
	void onLeftClicUp();
	void onScroll(float motion);
	void setCursor();

	bool mustShowAltOrigin();

	inline bool isBusy() { return selectedLayers.isBusy(); };

private:
	bool mouseIsHoveringAltOrigin();
	DraggablePoint* lookForHoveredDraggablePoint();
	bool canDragRotation();
public:
	GroupOfLayers layers;
	GroupOfLayers selectedLayers;
private:
	CursorType* usedCursor;
private:
	void computeHoveredLayerAndMouseRelPos();
	DraggablePoint* m_draggedPoint;
public:
	//Layer* m_activLayer;
	LayerID m_hoveredLayer;
	MousePositionRelativeToRect m_mousePosRelToHoveredLayer;
};