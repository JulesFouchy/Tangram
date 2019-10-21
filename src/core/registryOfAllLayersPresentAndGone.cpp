#include "registryOfAllLayersPresentAndGone.hpp"

#include "layer/loadedImageLayer.hpp"
#include "layer/copyLayer.hpp"
#include "layer/shaderLayer.hpp"
#include "layer/effectLayer.hpp"

RegistryOfAllLayersPresentAndGone::RegistryOfAllLayersPresentAndGone() {
	m_registry.push_back(nullptr);
}
RegistryOfAllLayersPresentAndGone::~RegistryOfAllLayersPresentAndGone() {
	for (size_t i = 0; i < m_registry.size(); ++i) {
		if (m_registry[i])
			freeLayer(i);
	}
}

Layer* RegistryOfAllLayersPresentAndGone::operator[](LayerID index) {
	if (index == 0)
		spdlog::error("[ RegistryOfAllLayersPresentAndGone::operator[] ] trying to access invalid index 0");
	return m_registry[index];
}

void RegistryOfAllLayersPresentAndGone::createLoadedImageLayer(const std::string& imgFilePath, LayerID registryID) {
	Layer* layer = new LoadedImageLayer(imgFilePath);
	addLayer(layer, registryID);
}

void RegistryOfAllLayersPresentAndGone::createCopyLayer(LayerID childLayerID, LayerID registryID) {
	Layer* layer = new CopyLayer(childLayerID);
	addLayer(layer, registryID);
}

void RegistryOfAllLayersPresentAndGone::createShaderLayer(const Ratio& ratio, unsigned int previewHeight, const std::string& fragmentShaderFilepath, LayerID registryID) {
	Layer* layer = new ShaderLayer(ratio, previewHeight, fragmentShaderFilepath);
	if (layer->createdSuccessfully())
		addLayer(layer, registryID);
	else
		delete layer;
}

void RegistryOfAllLayersPresentAndGone::createEffectLayer(const Ratio& ratio, unsigned int previewHeight, const std::string& fragmentShaderFilepath, LayerID targetLayerID, LayerID registryID) {
	Layer* layer = new EffectLayer(ratio, previewHeight, fragmentShaderFilepath, targetLayerID);
	if (layer->createdSuccessfully())
		addLayer(layer, registryID);
	else
		delete layer;
}

LayerID RegistryOfAllLayersPresentAndGone::addLayer(Layer* layer, LayerID index) {
	LayerID ID;
	// add in registry
	if (index == 0) {
		m_registry.push_back(layer);
		ID =  m_registry.size() - 1;
	}
	else {
		if (m_registry[index])
			spdlog::error("[RegistryOfAllLayersPresentAndGone::addLayer] Adding a layer at the ID of an alive layer !!!");
		m_registry[index] = layer;
		ID = index;
	}
	// add in alive-layer list
	m_aliveLayers.layers.addLayer(ID);
	return ID;
}

void RegistryOfAllLayersPresentAndGone::freeLayer(LayerID index) {
	delete m_registry[index];
}