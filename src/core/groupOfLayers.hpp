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

	inline int size() { return m_layers.size();	}
	Layer* operator[](int k);

	void addLayer(Layer* layer);
	void reorderLayer(Layer* layer, int newIndex);
	void removeLayer(Layer* layer);
	void removeLayer(int layerIndex);
	void removeAllLayers();

	bool contains(Layer* layer);
	inline bool isEmpty() { return m_layers.size() == 0; }

	void showFrames();

	bool isBusy();

	void showAltOrigin();
	glm::vec2 getAltOriginInTransformSpace();
	glm::vec2 getAltOriginInWindowSpace();
	void startDraggingAltOrigin();
	void resetAltOrigin();

	void startDraggingTranslation();
	void startDraggingScale(glm::vec2 originInDBspace);
	void startDraggingRotation();
	void startDraggingAspectRatio(glm::vec2 originInDBspace);
	void unlockUAspectRatio();
	void unlockVAspectRatio();
	void checkDragging();
	void endDragging();
	void changeDraggingScaleToAltOrigin();
	void revertDraggingScaleToInitialOrigin();
	void changeDraggingRatioToAltOrigin();
	void revertDraggingRatioToInitialOrigin();
	void switchDraggingToRatioFromScale();
	void switchDraggingToScaleFromRatio();

	void scale(float scaleFactor);

private:
	int getIndex(Layer* layer);
private:
	std::vector<Layer*> m_layers;
	GroupTransform m_transform;
};