#include "shaderLayer.hpp"

ShaderLayer::ShaderLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath)
	: Layer((float) previewWidth / previewHeight, fragmentFilePath), m_shader(fragmentFilePath, "res/shaders/vertex/standard.vert")
{
	// Initialize members
	m_displayTexture.Initialize(previewWidth, previewHeight, Texture2D::bytesPerPixel(RGBA), nullptr);
	drawShaderOnTexture();
}

ShaderLayer::~ShaderLayer() {

}

void ShaderLayer::drawShaderOnTexture() {
	
}