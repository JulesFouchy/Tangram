#pragma once

#include "layer.hpp"

#include "transforms/groupTransform.hpp"

#include <vector>

class GroupOfLayers {
	friend class DrawingBoard;
	friend class LayerList;
public:
	GroupOfLayers();
	~GroupOfLayers();

	void addLayer(Layer* layer);
	void removeLayer(Layer* layer);
	void removeAllLayers();

	bool contains(Layer* layer);

	void showFrames();

	void getActualAltOriginInDBspace();
	void showAltOrigin();

	void startDraggingTranslation();
	void startDraggingScale(glm::vec2 originInDBspace);
	void checkDragging();
	void endDragging();

private:
	std::vector<Layer*> m_layers;
	GroupTransform m_transform;
};