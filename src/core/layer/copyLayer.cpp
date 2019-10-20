#include "copyLayer.hpp"

#include "glm/gtc/matrix_transform.hpp"

CopyLayer::CopyLayer(LayerID childLayerID)
	: Layer(0.0f, "Copy of " + DrawingBoard::LayerRegistry()[childLayerID]->getName()), m_childLayerID(childLayerID)
{
	//Texture2D& childTexture = childLayer->getTexture();
	//m_displayTexture.Initialize(childTexture.getWidth(), childTexture.getHeight(), childTexture.getBPP(), childTexture.getPixels());
	m_transform = DrawingBoard::LayerRegistry()[m_childLayerID]->m_transform;
}

CopyLayer::~CopyLayer() {

}

void CopyLayer::createACopy() {
	Layer* newLayer = new CopyLayer(m_childLayerID);
	newLayer->setTransform(m_transform);
	DrawingBoard::LayerRegistry().addLayer(newLayer);
}

void CopyLayer::show(const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix) {
	DrawingBoard::LayerRegistry()[m_childLayerID]->show(glm::scale(m_transform.getMatrix(), glm::vec3(m_transform.getAspectRatio() / m_transform.getInitialAspectRatio(), 1.0f, 1.0f)), viewMatrix, projMatrix);
}

void CopyLayer::drawOnFrameBuffer_Preview(FrameBuffer& frameBuffer) {

}
void CopyLayer::drawOnFrameBuffer_Save(FrameBuffer& frameBuffer, int drawingBoardHeight) {
	DrawingBoard::LayerRegistry()[m_childLayerID]->drawOnFrameBuffer_Save(m_saveBuffer, drawingBoardHeight);
}

void CopyLayer::computeSaveBuffer(int drawingBoardHeight, RectTransform& transform) {
	int w = ceil(drawingBoardHeight * transform.getScale() * transform.getAspectRatio());
	int h = ceil(drawingBoardHeight * transform.getScale());
	m_saveBuffer.setTextureSize(w, h);
	drawOnFrameBuffer_Save(m_saveBuffer, drawingBoardHeight);
}

/*void CopyLayer::showForSaving(RectTransform& transform) {
	DrawingBoard::LayerRegistry()[m_childLayerID]->showForSaving(transform);
}*/