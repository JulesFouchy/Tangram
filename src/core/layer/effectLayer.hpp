#pragma once

#include "shaderLayer.hpp"
#include "core/layerID.hpp"

class EffectLayer : public ShaderLayer {
public:
	EffectLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath, LayerID targetLayerID);
	void createACopy() override;
	void drawShaderOnPreviewTexture() override;
	void setUniformsThatAreNotParametersOfTheFragShader_ForPreview() override;
	void setUniformsThatAreNotParametersOfTheFragShader_ForSaving(RectTransform& transform) override;
private:
	LayerID m_targetLayerID;
};