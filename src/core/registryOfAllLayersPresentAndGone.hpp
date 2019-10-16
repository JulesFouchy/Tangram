#pragma once

#include "layer/layer.hpp"
#include "layerList.hpp"
#include <vector>

#include "layerID.hpp"

class RegistryOfAllLayersPresentAndGone {
public:
	RegistryOfAllLayersPresentAndGone(); // beware : 0 is considered an invalid ID
	~RegistryOfAllLayersPresentAndGone();
	Layer* operator[](LayerID index);
	inline LayerList& AliveLayers() { return m_aliveLayers; }
	// Layer creation
	void createLoadedImageLayer(const std::string& imgFilePath, LayerID registryID = 0);
	void createCopyLayer(LayerID childLayerID, LayerID registryID = 0);
	void createShaderLayer(unsigned int previewWidth, unsigned int previewHeight, const std::string& fragmentShaderFilepath, LayerID registryID = 0);
	void createEffectLayer(unsigned int previewWidth, unsigned int previewHeight, const std::string& fragmentShaderFilepath, LayerID targetLayerID, LayerID registryID = 0);

	size_t addLayer(Layer* layer, LayerID index = 0); // if index == 0 (a.k.a. invalid ID), add a layer at a new index ; else re-add a layer at an index that was attributed and then freed, for example when removing a layer and then CTRL+Zing this action

private:
	void freeLayer(LayerID index);

private:
	std::vector<Layer*> m_registry;
	LayerList m_aliveLayers;
};