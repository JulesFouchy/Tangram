#pragma once

#include "shaderLayer.hpp"
#include "core/layerID.hpp"

class EffectLayer : public ShaderLayer {
public:
	EffectLayer(const Ratio& ratio, int previewHeight, const std::string& fragmentFilePath, LayerID targetLayerID);
	void createACopy() override;
	void computeSaveBuffer(int drawingBoardHeight, RectTransform& transform) override;
	void drawOnFrameBuffer_Preview(FrameBuffer& frameBuffer) override;
	void drawOnFrameBuffer_Save(FrameBuffer& frameBuffer, int DrawingBoardHeight) override;
	void setUniformsThatAreNotParametersOfTheFragShader_Preview() override;
	void setUniformsThatAreNotParametersOfTheFragShader_Save(int drawingBoardHeight) override;
private:
	LayerID m_targetLayerID;
	FrameBuffer m_targetSaveBuffer;
};