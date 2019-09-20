#include "copyLayer.hpp"

CopyLayer::CopyLayer(Layer* childLayer)
	: Layer(childLayer->getTexture().getAspectRatio(), "Copy of " + childLayer->getName()), m_childLayer(childLayer)
{
	Texture2D& childTexture = childLayer->getTexture();
	m_displayTexture.Initialize(childTexture.getWidth(), childTexture.getHeight(), childTexture.getBPP(), childTexture.getPixels());
	m_transform = childLayer->m_transform;
}

CopyLayer::~CopyLayer() {

}