#pragma once

#include "layer.hpp"
#include "core/layerID.hpp"
#include "core/drawingBoard.hpp"

class CopyLayer : public Layer {
public:
	CopyLayer(LayerID childLayerID);
	~CopyLayer();

	void createACopy() override;

	void show(const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix) override;
	//void showForSaving(RectTransform& transform) override;

	inline Texture2D& getTexture_Preview() override { return DrawingBoard::LayerRegistry()[m_childLayerID]->getTexture_Preview(); }
	inline FrameBuffer& getFrameBuffer() override { return DrawingBoard::LayerRegistry()[m_childLayerID]->getFrameBuffer(); }

	void drawOnFrameBuffer_Preview(FrameBuffer& frameBuffer) override;
	void drawOnFrameBuffer_Save(FrameBuffer& frameBuffer, int drawingBoardHeight) override;
	inline void computePreviewBuffer() {};
	void computeSaveBuffer(int drawingBoardHeight, RectTransform& transform) override;
private:
	LayerID m_childLayerID;
};