#pragma once

#include "drawingBoard.hpp"
#include <vector>

class DrawingBoard;
class Layer;

class LayerList {
public:
	LayerList();
	~LayerList();

	void show();
	void showFrames();

	void addLayer(std::string imgFilePath, DrawingBoard* dbPointer);
	Layer* getActivLayer();
	void setActivLayer(int layerIndex);

public:
	std::vector<Layer*> layers;
private:
	int m_activLayerIndex;
};