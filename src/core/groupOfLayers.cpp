#include "groupOfLayers.hpp"

#include "UI/log.hpp"

#include "drawingBoard.hpp"

#include <algorithm>

void GroupOfLayers::reload() {
	for (LayerID layerID : m_layerIDs)
		DrawingBoard::LayerRegistry()[layerID]->reload();
}

LayerID GroupOfLayers::operator[](int k) {
	return m_layerIDs[k];
}

void GroupOfLayers::addLayer(LayerID layerID) {
	//Check for doublons
	for (LayerID otherLayerID : m_layerIDs) {
		if (otherLayerID == layerID) {
			spdlog::warn("[Group of Layers] trying to add a layer that was already in it : {}", DrawingBoard::LayerRegistry()[layerID]->getName());
			return;
		}
	}
	//Add layer
	m_layerIDs.push_back(layerID);
}

void GroupOfLayers::moveLayer(int fromIndex, int toIndex) {
	LayerID movedLayerID = m_layerIDs[fromIndex];
	m_layerIDs.insert(m_layerIDs.begin() + toIndex, movedLayerID);
	removeLayerByIndexInGroup(fromIndex < toIndex ? fromIndex : fromIndex + 1);
}

void GroupOfLayers::reorderLayer(LayerID layerID, int newIndex) {
	size_t layerIndex = getIndex(layerID);
	//if (layerIndex >= 0) {
		DrawingBoard::history.beginUndoGroup();
			DrawingBoard::history.addAction(Action(
				// DO action
				[this, layerIndex, newIndex]()
			{
				moveLayer(layerIndex, newIndex);
			},
				// UNDO action
				[this, layerIndex, newIndex]()
			{
				if (layerIndex > newIndex) {
					moveLayer(newIndex, layerIndex + 1);
				}
				else {
					moveLayer(newIndex-1, layerIndex);
				}
			}
			));
		DrawingBoard::history.endUndoGroup();
		moveLayer(layerIndex, newIndex);
	//}
	//else {
	//	spdlog::warn("[Group of Layers] trying to reorder a layer that isn't in the group : {}", layer->getName());
	//}
}

void GroupOfLayers::removeLayerByRegisterID(LayerID layerID) {
	size_t index = getIndex(layerID);
	//if (index >= 0) {
	removeLayerByIndexInGroup(index);
	//}
	//else {
	//	spdlog::warn("[Group of Layers] asked to remove a layer that wasn't there : {}", layer->getName());
	//}
}

void GroupOfLayers::removeLayerByIndexInGroup(size_t layerIndex) {
	m_layerIDs.erase(m_layerIDs.begin() + layerIndex);
}

void GroupOfLayers::removeAllLayers() {
	m_layerIDs.resize(0);
}

bool GroupOfLayers::contains(LayerID layerID) {
	return std::find(m_layerIDs.begin(), m_layerIDs.end(), layerID) != m_layerIDs.end();
}

void GroupOfLayers::showFrames() {
	for (LayerID layerID : m_layerIDs) {
		DrawingBoard::LayerRegistry()[layerID]->showFrame();
	}
}

bool GroupOfLayers::isBusy() {
	bool busy = m_transform.isBusy();
	for (LayerID layerID : m_layerIDs) {
		busy |= DrawingBoard::LayerRegistry()[layerID]->m_transform.isBusy();
	}
	return busy;
}

void GroupOfLayers::showAltOrigin() {
	if (m_layerIDs.size() == 1) {
		DrawingBoard::LayerRegistry()[m_layerIDs[0]]->m_transform.showAltOrigin();
	}
	else if (m_layerIDs.size() > 1) {
		m_transform.showAltOrigin();
	}
	else {
		spdlog::warn("[Group of Layers] showAltOrigin was called but there is actually no layer in the group !");
	}
}

glm::vec2 GroupOfLayers::getAltOriginInTransformSpace() {
	if (m_layerIDs.size() == 1) {
		return DrawingBoard::LayerRegistry()[m_layerIDs[0]]->m_transform.getAltOriginInTransformSpace();
	}
	else if (m_layerIDs.size() > 1) {
		return m_transform.getAltOriginInTransformSpace();
	}
	else {
		spdlog::warn("[Group of Layers] getAltOriginInTransformSpace was called but there is actually no layer in the group !");
		return glm::vec2(0.0f);
	}
}

glm::vec2 GroupOfLayers::getAltOriginInWindowSpace() {
	if (m_layerIDs.size() == 1) {
		return DrawingBoard::LayerRegistry()[m_layerIDs[0]]->m_transform.getAltOriginInWindowSpace();
	}
	else if (m_layerIDs.size() > 1) {
		return m_transform.getAltOriginInWindowSpace();
	}
	else {
		spdlog::warn("[Group of Layers] getAltOriginInWindowSpace was called but there is actually no layer in the group !");
		return glm::vec2(0.0f);
	}
}

void GroupOfLayers::startDraggingAltOrigin() {
	if (m_layerIDs.size() == 1) {
		DrawingBoard::LayerRegistry()[m_layerIDs[0]]->m_transform.startDraggingAltOrigin();
	}
	else if (m_layerIDs.size() > 1) {
		m_transform.startDraggingAltOrigin();
	}
	else {
		spdlog::warn("[Group of Layers] startDraggingAltOrigin was called but there is actually no layer in the group !");
	}
}

void GroupOfLayers::resetAltOrigin() {
	if (m_layerIDs.size() == 1) {
		DrawingBoard::LayerRegistry()[m_layerIDs[0]]->m_transform.setAltOrigin(glm::vec2(0.0f), true);
	}
	else if (m_layerIDs.size() > 1) {
		m_transform.setAltOrigin(glm::vec2(0.0f), true);
		//Set in the middle of all layers
		glm::vec2 newOriginInDBspace = glm::vec2(0.0f);
		for (LayerID layerID : m_layerIDs)
			newOriginInDBspace = glm::vec4(newOriginInDBspace, 0.0f, 0.0f) + DrawingBoard::LayerRegistry()[layerID]->m_transform.getMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		newOriginInDBspace /= m_layerIDs.size();
		m_transform.setAltOrigin(glm::vec4(newOriginInDBspace, 0.0f, 1.0f), true); //no need to convert back to transform space bc m_transform.getMatrix() == identity
	}
	else {
		spdlog::warn("[Group of Layers] resetAltOrigin was called but there is actually no layer in the group !");
	}
}

void GroupOfLayers::startDraggingTranslation() {
	m_transform.startDraggingTranslation();
	for (LayerID layerID : m_layerIDs) {
		DrawingBoard::LayerRegistry()[layerID]->m_transform.startDraggingTranslation();
	}
}

void GroupOfLayers::startDraggingRotation() {
	if (m_layerIDs.size() == 1) {
		DrawingBoard::LayerRegistry()[m_layerIDs[0]]->m_transform.startDraggingRotation();
	}
	else if (m_layerIDs.size() > 1) {
		m_transform.startDraggingRotation();
		for (LayerID layerID : m_layerIDs) {
			DrawingBoard::LayerRegistry()[layerID]->m_transform.startDraggingRotation(m_transform.getAltOriginInDrawingBoardSpace());
		}
	}
	else {
		spdlog::warn("[Group of Layers] startDraggingRotation was called but there is actually no layer in the group !");
	}
}

void GroupOfLayers::startDraggingScale(LayerID leadLayerID, glm::vec2 originInDBspace, glm::vec2 uAxis, glm::vec2 vAxis, bool unlockU, bool unlockV) {
	m_transform.startDraggingScale(&(DrawingBoard::LayerRegistry()[leadLayerID]->m_transform), originInDBspace, unlockU, unlockV);
	for (LayerID layerID : m_layerIDs) {
		DrawingBoard::LayerRegistry()[layerID]->m_transform.startDraggingScale(m_transform.getAspectRatioDraggingInfos(), originInDBspace, layerID != leadLayerID);
	}
}

void GroupOfLayers::checkDragging() {
	m_transform.checkDragging();
	for (LayerID layerID : m_layerIDs) {
		DrawingBoard::LayerRegistry()[layerID]->m_transform.checkDragging();
	}
}

void GroupOfLayers::endDragging() {
		m_transform.endDragging();
		for (LayerID layerID : m_layerIDs) {
			DrawingBoard::LayerRegistry()[layerID]->m_transform.endDragging();
		}
}

void GroupOfLayers::pushStateInHistoryAtTheEndOfDragging() {
	DrawingBoard::history.beginUndoGroup();
		m_transform.pushStateInHistoryAtTheEndOfDragging();
		for (LayerID layerID : m_layerIDs) {
			DrawingBoard::LayerRegistry()[layerID]->m_transform.pushStateInHistoryAtTheEndOfDragging();
		}
	DrawingBoard::history.endUndoGroup();
}

void GroupOfLayers::changeDraggingCenterToAltOrigin() {
	if (m_layerIDs.size() == 1) {
		DrawingBoard::LayerRegistry()[m_layerIDs[0]]->m_transform.changeDraggingCenterToAltOrigin();
	}
	else if (m_layerIDs.size() > 1) {
		m_transform.changeDraggingCenterToAltOrigin();
		for (LayerID layerID : m_layerIDs) {
			Layer* layer = DrawingBoard::LayerRegistry()[layerID];
			layer->m_transform.changeDraggingCenter(layer->m_transform.getInverseMatrix() * glm::vec4(m_transform.getAltOriginInDrawingBoardSpace(), 0.0f, 1.0f));
		}
	}
	else {
		spdlog::warn("[Group of Layers] changeDraggingCenterToAltOrigin was called but there is actually no layer in the group !");
	}
}

void GroupOfLayers::revertDraggingCenterToInitialOrigin() {
	m_transform.revertDraggingCenterToInitialOrigin();
	for (LayerID layerID : m_layerIDs) {
		DrawingBoard::LayerRegistry()[layerID]->m_transform.revertDraggingCenterToInitialOrigin();
	}
}

void GroupOfLayers::scale(float scaleFactor, bool bPushChangeInHistory) {
	if (bPushChangeInHistory)
		DrawingBoard::history.beginUndoGroup();
	if (m_layerIDs.size() == 1) {
		DrawingBoard::LayerRegistry()[m_layerIDs[0]]->m_transform.scale(scaleFactor, DrawingBoard::LayerRegistry()[m_layerIDs[0]]->m_transform.getAltOriginInDrawingBoardSpace(), bPushChangeInHistory);
	}
	else if (m_layerIDs.size() > 1) {
		m_transform.scale(scaleFactor, m_transform.getAltOriginInDrawingBoardSpace(), bPushChangeInHistory);
		for (LayerID layerID : m_layerIDs) {
			DrawingBoard::LayerRegistry()[layerID]->m_transform.scale(scaleFactor, glm::vec4(m_transform.getAltOriginInDrawingBoardSpace(), 0.0f, 1.0f), bPushChangeInHistory);
		}
	}
	else {
		spdlog::warn("[Group of Layers] scale was called but there is actually no layer in the group !");
	}
	if (bPushChangeInHistory)
		DrawingBoard::history.endUndoGroup();
}

size_t GroupOfLayers::getIndex(LayerID layerID) {
	for (size_t k = 0; k < m_layerIDs.size(); ++k) {
		if (m_layerIDs[k] == layerID)
			return k;
	}
	spdlog::error("[GroupOfLayers::getIndex] layer {} not found in the group", layerID);
	return -1;
}