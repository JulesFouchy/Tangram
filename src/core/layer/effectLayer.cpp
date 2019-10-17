#include "effectLayer.hpp"

#include "core/drawingBoard.hpp"

EffectLayer::EffectLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath, LayerID targetLayerID)
	: ShaderLayer(previewWidth, previewHeight, fragmentFilePath, ZERO_TO_ONE__ZERO_TO_ONE), m_targetLayerID(targetLayerID)
{
	drawShaderOnPreviewTexture();
}

void EffectLayer::createACopy() {
	EffectLayer* newLayer = new EffectLayer(getTexture().getWidth(), getTexture().getHeight(), m_shader.getFragmentFilepath(), m_targetLayerID);
	newLayer->setTransform(m_transform);
	newLayer->m_uniforms = m_uniforms;
	newLayer->drawShaderOnPreviewTexture();
	DrawingBoard::LayerRegistry().addLayer(newLayer);
}

void EffectLayer::drawShaderOnPreviewTexture() {
	ShaderLayer::drawShaderOnPreviewTexture();
	//
	Texture2D& targetTexture = DrawingBoard::LayerRegistry()[m_targetLayerID]->getTexture();
	targetTexture.unbind();
}

void EffectLayer::setUniformsThatAreNotParametersOfTheFragShader_ForPreview() {
	ShaderLayer::setUniformsThatAreNotParametersOfTheFragShader_ForPreview();
	//
	Texture2D& targetTexture = DrawingBoard::LayerRegistry()[m_targetLayerID]->getTexture();
	targetTexture.bind();
	m_shader.setUniform1i("u_textureSlot", targetTexture.getSlot());
}

void EffectLayer::setUniformsThatAreNotParametersOfTheFragShader_ForSaving(RectTransform& transform) {
	ShaderLayer::setUniformsThatAreNotParametersOfTheFragShader_ForSaving(transform);
	//
	Texture2D& targetTexture = DrawingBoard::LayerRegistry()[m_targetLayerID]->getSaveTexture();
	targetTexture.bind();
	m_shader.setUniform1i("u_textureSlot", targetTexture.getSlot());
}