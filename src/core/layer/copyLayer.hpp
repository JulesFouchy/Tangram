#pragma once

#include "layer.hpp"
#include "core/layerID.hpp"
#include "core/drawingBoard.hpp"

class CopyLayer : public Layer {
public:
	CopyLayer(LayerID childLayerID);
	~CopyLayer();

	void show(const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix) override;
	void showForSaving(RectTransform& transform) override;

	inline Texture2D& getTexture() override { return DrawingBoard::LayerRegistry()[m_childLayerID]->getTexture(); }
	inline FrameBuffer& getFrameBuffer() override { return DrawingBoard::LayerRegistry()[m_childLayerID]->getFrameBuffer(); }
private:
	LayerID m_childLayerID;
};