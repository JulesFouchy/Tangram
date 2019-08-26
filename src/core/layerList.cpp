#include "layerList.hpp"

#include "drawingBoard.hpp"

LayerList::LayerList()
	: m_activLayer(nullptr), m_hoveredLayer(nullptr), m_mousePosRelToHoveredLayer(OUTSIDE)
{
}

LayerList::~LayerList() {

}

void LayerList::update() {
	computeHoveredLayerAndMouseRelPos();
	if(m_activLayer)
		m_activLayer->m_transform.checkDraggingTranslation();
}

void LayerList::show() {
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->show();
	}
}

void LayerList::showFrames() {
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->showFrame();
	}
}

void LayerList::addLayer(std::string imgFilePath) {
	layers.push_back(new Layer(imgFilePath));
	setActivLayer(layers.size() - 1);
}

Layer* LayerList::getActivLayer() {
	return m_activLayer;
}
void LayerList::setActivLayer(int layerIndex) {
	m_activLayer = layers[layerIndex];
}

void LayerList::computeHoveredLayerAndMouseRelPos() {
	m_hoveredLayer = nullptr;
	for (int k = layers.size()-1; k >= 0; --k) {
		m_mousePosRelToHoveredLayer = layers[k]->m_transform.getMouseRelativePosition();
		if (m_mousePosRelToHoveredLayer != OUTSIDE) {
			m_hoveredLayer = layers[k];
			break;
		}
	}
}

void LayerList::onLeftClicDown() {
	if (m_hoveredLayer) {
		m_activLayer = m_hoveredLayer;
		m_isHandlingAnInput = true;

		switch (m_mousePosRelToHoveredLayer) {
		case INSIDE:
			m_hoveredLayer->m_transform.startDraggingTranslation();
		break;

		}
	}
}

void LayerList::onLeftClicUp() {
	m_activLayer->m_transform.endDraggingTranslation();
	m_isHandlingAnInput = false;
}