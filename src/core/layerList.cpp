#include "layerList.hpp"

#include "drawingBoard.hpp"

LayerList::LayerList()
	: m_activLayerIndex(-1)
{
}

LayerList::~LayerList() {

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
	return layers[m_activLayerIndex];
}
void LayerList::setActivLayer(int layerIndex) {
	m_activLayerIndex = layerIndex;
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