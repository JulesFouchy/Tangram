#include "copyLayer.hpp"

#include "glm/gtc/matrix_transform.hpp"

CopyLayer::CopyLayer(Layer* childLayer)
	: Layer(0.0f, "Copy of " + childLayer->getName()), m_childLayer(childLayer)
{
	//Texture2D& childTexture = childLayer->getTexture();
	//m_displayTexture.Initialize(childTexture.getWidth(), childTexture.getHeight(), childTexture.getBPP(), childTexture.getPixels());
	m_transform = childLayer->m_transform;
}

CopyLayer::~CopyLayer() {

}

void CopyLayer::show(const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix) {
	m_childLayer->show(glm::scale(m_transform.getMatrix(), glm::vec3(m_transform.getAspectRatio() / m_transform.getInitialAspectRatio(), 1.0f, 1.0f)), viewMatrix, projMatrix);
}
void CopyLayer::showInDrawingBoardSpace() {
	m_childLayer->showInDrawingBoardSpace(m_transform.getMatrix());
}