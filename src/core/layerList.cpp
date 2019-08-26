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

void LayerList::addLayer(std::string imgFilePath, DrawingBoard* dbPointer) {
	layers.push_back(new Layer(imgFilePath, dbPointer));
	setActivLayer(layers.size() - 1);
}

Layer* LayerList::getActivLayer() {
	return layers[m_activLayerIndex];
}
void LayerList::setActivLayer(int layerIndex) {
	m_activLayerIndex = layerIndex;
}