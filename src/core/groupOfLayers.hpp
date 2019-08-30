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
	inline bool isEmpty() { return m_layers.size() == 0; }

	void showFrames();

	void showAltOrigin();
	glm::vec2 getAltOriginInWindowSpace();
	void startDraggingAltOrigin();
	void resetAltOrigin();

	void startDraggingTranslation();
	void startDraggingScale(glm::vec2 originInDBspace);
	void startDraggingRotation();
	void checkDragging();
	void endDragging();
	void changeDraggingScaleToAltOrigin();
	void revertDraggingScaleToInitialOrigin();

	void scale(float scaleFactor);

private:
	std::vector<Layer*> m_layers;
	GroupTransform m_transform;
};