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
	void startDraggingAspectRatio(glm::vec2 originInDBspace, glm::vec2 uAxis, glm::vec2 vAxis);
	void unlockUAspectRatio();
	void unlockVAspectRatio();
	void checkDragging();
	void endDragging();
	void pushStateInHistory();
	void changeDraggingCenterToAltOrigin();
	void revertDraggingCenterToInitialOrigin();
	void switchDraggingToRatioFromScale();
	void switchDraggingToScaleFromRatio();

	void scaleAndPushChangeToHistory(float scaleFactor);

private:
	int getIndex(Layer* layer);
private:
	std::vector<Layer*> m_layers;
	GroupTransform m_transform;
};