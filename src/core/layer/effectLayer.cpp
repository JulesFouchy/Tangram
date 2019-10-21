#include "effectLayer.hpp"

#include "core/drawingBoard.hpp"

EffectLayer::EffectLayer(const Ratio& ratio, int previewHeight, const std::string& fragmentFilePath, LayerID targetLayerID)
	: ShaderLayer(ratio, previewHeight, fragmentFilePath, ZERO_TO_ONE__ZERO_TO_ONE), m_targetLayerID(targetLayerID)
{
	drawOnFrameBuffer_Preview(m_previewBuffer);
}

void EffectLayer::createACopy() {
	EffectLayer* newLayer = new EffectLayer(getTexture_Preview().getWidth(), getTexture_Preview().getHeight(), m_shader.getFragmentFilepath(), m_targetLayerID);
	newLayer->setTransform(m_transform);
	newLayer->m_uniforms = m_uniforms;
	newLayer->onChange();
	DrawingBoard::LayerRegistry().addLayer(newLayer);
}

void EffectLayer::computeSaveBuffer(int drawingBoardHeight, RectTransform& transform) {
	int w = ceil(drawingBoardHeight * transform.getScale() * transform.getAspectRatio());
	int h = ceil(drawingBoardHeight * transform.getScale());
	m_saveBuffer.setTextureSize(w, h);
	m_targetSaveBuffer.setTextureSize(w, h);
	DrawingBoard::LayerRegistry()[m_targetLayerID]->drawOnFrameBuffer_Save(m_targetSaveBuffer, drawingBoardHeight);
	drawOnFrameBuffer_Save(m_saveBuffer, drawingBoardHeight);
}

void EffectLayer::drawOnFrameBuffer_Preview(FrameBuffer& frameBuffer) {
	ShaderLayer::drawOnFrameBuffer_Preview(frameBuffer);
	//
	DrawingBoard::LayerRegistry()[m_targetLayerID]->getTexture_Preview().unbind();
}

void EffectLayer::drawOnFrameBuffer_Save(FrameBuffer& frameBuffer, int drawingBoardHeight) {
	ShaderLayer::drawOnFrameBuffer_Save(frameBuffer, drawingBoardHeight);
	//
	DrawingBoard::LayerRegistry()[m_targetLayerID]->getTexture_Save().unbind();
}

void EffectLayer::setUniformsThatAreNotParametersOfTheFragShader_Preview() {
	ShaderLayer::setUniformsThatAreNotParametersOfTheFragShader_Preview();
	//
	Texture2D& targetTexture = DrawingBoard::LayerRegistry()[m_targetLayerID]->getTexture_Preview();
	targetTexture.bind();
	m_shader.setUniform1i("TNGu_textureSlot", targetTexture.getSlot());
}

void EffectLayer::setUniformsThatAreNotParametersOfTheFragShader_Save(int drawingBoardHeight) {
	ShaderLayer::setUniformsThatAreNotParametersOfTheFragShader_Save(drawingBoardHeight);
	//
	Texture2D& targetTexture = m_targetSaveBuffer.getTexture();
	targetTexture.bind();
	m_shader.setUniform1i("u_textureSlot", targetTexture.getSlot());
}