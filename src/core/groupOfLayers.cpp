#include "groupOfLayers.hpp"

#include "UI/log.hpp"

#include <algorithm>

GroupOfLayers::GroupOfLayers() {

}

GroupOfLayers::~GroupOfLayers() {

}

void GroupOfLayers::addLayer(Layer* layer) {
	//Check for doublons
	for (Layer* otherLayer : m_layers) {
		if (otherLayer == layer) {
			spdlog::warn("[Group of Layers] trying to add a layer that was already in it : {}", layer->getName());
			return;
		}
	}
	//Add layer
	m_layers.push_back(layer);
}

void GroupOfLayers::removeLayer(Layer* layer) {
	for (int k = 0; k < m_layers.size(); ++k) {
		if (m_layers[k] == layer) {
			m_layers.erase(m_layers.begin() + k);
			return;
		}
	}
	spdlog::warn("[Group of Layers] asked to remove a layer that wasn't in it : {}", layer->getName());
}

void GroupOfLayers::removeAllLayers() {
	m_layers.resize(0);
}

bool GroupOfLayers::contains(Layer* layer) {
	return std::find(m_layers.begin(), m_layers.end(), layer) != m_layers.end();
}

void GroupOfLayers::showFrames() {
	for (int k = 0; k < m_layers.size(); ++k) {
		m_layers[k]->showFrame();
	}
}

void GroupOfLayers::showAltOrigin() {
	if (m_layers.size() == 1) {
		m_layers[0]->m_transform.showAltOrigin();
	}
	else if (m_layers.size() > 1) {
		m_transform.showAltOrigin();
	}
	else {
		spdlog::warn("[Group of Layers] showAltOrigin was called but there is actually no layer in the group !");
	}
}

glm::vec2 GroupOfLayers::getAltOriginInWindowSpace() {
	if (m_layers.size() == 1) {
		return m_layers[0]->m_transform.getAltOriginInWindowSpace();
	}
	else if (m_layers.size() > 1) {
		return m_transform.getAltOriginInWindowSpace();
	}
	else {
		spdlog::warn("[Group of Layers] getAltOrigin was called but there is actually no layer in the group !");
		return glm::vec2(0.0f);
	}
}

void GroupOfLayers::startDraggingAltOrigin() {
	if (m_layers.size() == 1) {
		m_layers[0]->m_transform.startDraggingAltOrigin();
	}
	else if (m_layers.size() > 1) {
		m_transform.startDraggingAltOrigin();
	}
	else {
		spdlog::warn("[Group of Layers] startDraggingAltOrigin was called but there is actually no layer in the group !");
	}
}

void GroupOfLayers::resetAltOrigin() {
	if (m_layers.size() == 1) {
		m_layers[0]->m_transform.setAltOrigin(glm::vec2(0.0f));
	}
	else if (m_layers.size() > 1) {
		m_transform.reset();
		//Set in the middle of all layers
		glm::vec2 newOriginInDBspace = glm::vec2(0.0f);
		for (Layer* layer : m_layers)
			newOriginInDBspace = glm::vec4(newOriginInDBspace,0.0f, 0.0f) + layer->m_transform.getMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		newOriginInDBspace /= m_layers.size();
		m_transform.setAltOrigin(glm::vec4(newOriginInDBspace, 0.0f, 1.0f)); //no need to convert back to transform space bc m_transform.getMatrix() == identity
	}
	else {
		spdlog::warn("[Group of Layers] resetAltOrigin was called but there is actually no layer in the group !");
	}
}

void GroupOfLayers::startDraggingTranslation() {
	m_transform.startDraggingTranslation();
	for (int k = 0; k < m_layers.size(); ++k) {
		m_layers[k]->m_transform.startDraggingTranslation();
	}
}

void GroupOfLayers::startDraggingScale(glm::vec2 originInDBspace) {
	m_transform.startDraggingScale(m_transform.getInverseMatrix() * glm::vec4(originInDBspace, 0.0f, 1.0f));
	for (int k = 0; k < m_layers.size(); ++k) {
		m_layers[k]->m_transform.startDraggingScale(m_layers[k]->m_transform.getInverseMatrix() * glm::vec4(originInDBspace, 0.0f, 1.0f));
	}
}

void GroupOfLayers::startDraggingRotation() {
	if (m_layers.size() == 1) {
		m_layers[0]->m_transform.startDraggingRotation();
	}
	else if (m_layers.size() > 1) {
		m_transform.startDraggingRotation();
		for (int k = 0; k < m_layers.size(); ++k) {
			m_layers[k]->m_transform.startDraggingRotation(m_transform.getAltOriginInDrawingBoardSpace());
		}
	}
	else {
		spdlog::warn("[Group of Layers] startDraggingRotation was called but there is actually no layer in the group !");
	}
}

void GroupOfLayers::checkDragging() {
	m_transform.checkDragging();
	for (int k = 0; k < m_layers.size(); ++k) {
		m_layers[k]->m_transform.checkDragging();
	}
}

void GroupOfLayers::endDragging() {
	m_transform.endDragging();
	for (int k = 0; k < m_layers.size(); ++k) {
		m_layers[k]->m_transform.endDragging();
	}
}

void GroupOfLayers::changeDraggingScaleToAltOrigin() {
	if (m_layers.size() == 1) {
		m_layers[0]->m_transform.changeDraggingScaleToAltOrigin();
	}
	else if (m_layers.size() > 1) {
		m_transform.changeDraggingScaleToAltOrigin();
		for (int k = 0; k < m_layers.size(); ++k) {
			m_layers[k]->m_transform.changeDraggingScaleOrigin(m_layers[k]->m_transform.getInverseMatrix() * glm::vec4(m_transform.getAltOriginInDrawingBoardSpace(), 0.0f, 1.0f));
		}
	}
	else {
		spdlog::warn("[Group of Layers] changeDraggingScaleToAltOrigin was called but there is actually no layer in the group !");
	}
}

void GroupOfLayers::revertDraggingScaleToInitialOrigin() {
	m_transform.revertDraggingScaleToInitialOrigin();
	for (int k = 0; k < m_layers.size(); ++k) {
		m_layers[k]->m_transform.revertDraggingScaleToInitialOrigin();
	}
}

void GroupOfLayers::scale(float scaleFactor) {
	if (m_layers.size() == 1) {
		m_layers[0]->m_transform.scale(scaleFactor, m_layers[0]->m_transform.getAltOriginInDrawingBoardSpace());
	}
	else if (m_layers.size() > 1) {
		m_transform.scale(scaleFactor, m_transform.getAltOriginInDrawingBoardSpace());
		for (int k = 0; k < m_layers.size(); ++k) {
			m_layers[k]->m_transform.scale(scaleFactor, glm::vec4(m_transform.getAltOriginInDrawingBoardSpace(), 0.0f, 1.0f));
		}
	}
	else {
		spdlog::warn("[Group of Layers] scale was called but there is actually no layer in the group !");
	}
}