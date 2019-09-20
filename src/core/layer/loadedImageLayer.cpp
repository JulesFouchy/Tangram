#include "loadedImageLayer.hpp"

#include "stb_image/stb_image.h"

LoadedImageLayer::LoadedImageLayer(const std::string& imgFilePath)
	: Layer(0.1f, imgFilePath), m_filePath(imgFilePath)
{
	spdlog::info("[Opening image] " + imgFilePath);
	// Load file 
	stbi_set_flip_vertically_on_load(1);
	int width, height, BPP;
	unsigned char* pixels = stbi_load(imgFilePath.c_str(), &width, &height, &BPP, Texture2D::bytesPerPixel(RGBA));
	if (!pixels)
		spdlog::warn("Couldn't open " + imgFilePath);
	// Init texture
	m_displayTexture.Initialize(width, height, BPP, pixels);
	Log::separationLine();
	// Initialize members
	m_transform = RectTransform((float) width / (float) height);
	// 
	
}

LoadedImageLayer::~LoadedImageLayer() {
	spdlog::info("[Loaded Image Layer Destructed] " + getName());
	if (m_pixels)
		stbi_image_free(m_pixels);
}