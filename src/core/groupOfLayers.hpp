#pragma once

#include "layer/layer.hpp"

#include "transforms/groupTransform.hpp"

#include "layerID.hpp"

#include <vector>

class GroupOfLayers {
	friend class DrawingBoard;
	friend class LayerList;
public:
	GroupOfLayers() = default;
	~GroupOfLayers() = default;

	void reload();

	inline int size() { return m_layerIDs.size();	}
	LayerID operator[](int k);

	void addLayer(LayerID layerID);
	void reorderLayer(LayerID layerID, int newIndex);
	void removeLayerByRegisterID(LayerID layerID);
	void removeLayerByIndexInGroup(size_t layerIndex);
	void removeAllLayers();

	bool contains(LayerID layerID);
	inline bool isEmpty() { return m_layerIDs.size() == 0; }

	void showFrames();

	bool isBusy();

	void showAltOrigin();
	glm::vec2 getAltOriginInTransformSpace();
	glm::vec2 getAltOriginInWindowSpace();
	void startDraggingAltOrigin();
	void resetAltOrigin();

	void startDraggingTranslation();
	void startDraggingScale(LayerID leadLayerID, glm::vec2 originInDBspace, glm::vec2 uAxis, glm::vec2 vAxis, bool unlockU, bool unlockV);
	void startDraggingRotation();
	void checkDragging();
	void endDragging();
	void pushStateInHistoryAtTheEndOfDragging();
	void changeDraggingCenterToAltOrigin();
	void revertDraggingCenterToInitialOrigin();

	void scale(float scaleFactor, bool bPushChangeInHistory = false);

private:
	size_t getIndex(LayerID layerID);
	void moveLayer(int fromIndex, int toIndex);
private:
	std::vector<LayerID> m_layerIDs;
	GroupTransform m_transform;
};