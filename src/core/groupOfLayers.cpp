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

void GroupOfLayers::getActualAltOriginInDBspace() {

}

void GroupOfLayers::showAltOrigin() {
	if (m_layers.size() == 1) {
		m_layers[0]->m_transform.showAltOrigin();
	}
	else if (m_layers.size() > 1) {
		m_transform.showAltOrigin();
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