#include "loadedImageLayer.hpp"

#include "stb_image/stb_image.h"

#include "core/drawingBoard.hpp"

LoadedImageLayer::LoadedImageLayer(const std::string& imgFilePath)
	: Layer(0.1f, imgFilePath), m_filePath(imgFilePath)
	 //      î dummy initialization of Layer's aspectRatio because we need to read image to know it
{
	spdlog::info("[Opening image] " + imgFilePath);
	// Load file 
	stbi_set_flip_vertically_on_load(1);
	int width, height, BPP;
	unsigned char* pixels = stbi_load(imgFilePath.c_str(), &width, &height, &BPP, Texture2D::bytesPerPixel(RGBA));
	if (!pixels)
		spdlog::warn("Couldn't open " + imgFilePath);
	Log::separationLine();
	// Initialize members
	m_renderBuffer.getTexture().Initialize(width, height, BPP, pixels);
	m_transform = RectTransform((float) width / height);	
}

LoadedImageLayer::~LoadedImageLayer() {
	spdlog::info("[Loaded Image Layer Destructed] " + getName());
	if (m_pixels)
		stbi_image_free(m_pixels);
}

void LoadedImageLayer::showForSaving(RectTransform& transform) {
	show(transform.getMatrix(), glm::mat4x4(1.0f), DrawingBoard::transform.getProjectionMatrix());
}