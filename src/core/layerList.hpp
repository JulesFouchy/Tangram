#pragma once

#include "layer/layer.hpp"

#include "groupOfLayers.hpp"

#include <vector>

#include "UI/cursor.hpp"

#include "UI/keys.hpp"

class LayerList {
public:
	LayerList();
	~LayerList();

	void update();

	void show(glm::mat4x4 viewMatrix, glm::mat4x4 projMatrix);
	void showInDrawingBoardSpace();
	void showSelectedLayerGUI();

	void createLoadedImageLayer(const std::string& imgFilePath);
	void createCopyLayer(Layer* childLayer);
	void createShaderLayer(const std::string& fragmentShaderFilepath);
	Layer* getLayer(int index);
	void setSelectedLayer(Layer* layer);

	void onKeyDown(Key key);
	void onKeyUp(Key key);
	void onDoubleLeftClic();
	void onLeftClicDown();
	void onLeftClicUp();
	void onScroll(float motion);
	void setCursor();

	bool mustShowAltOrigin();

	inline bool isHandlingAnInput() { return selectedLayers.isBusy(); };

private:
	bool mouseIsHoveringAltOrigin();
	bool canDragRotation();
public:
	GroupOfLayers layers;
	GroupOfLayers selectedLayers;
private:
	CursorType* usedCursor;
private:
	void computeHoveredLayerAndMouseRelPos();
public:
	//Layer* m_activLayer;
	Layer* m_hoveredLayer;
	MousePositionRelativeToRect m_mousePosRelToHoveredLayer;
};