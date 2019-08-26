#include "loadedImage.hpp"

#include "stb_image/stb_image.h"

#include "spdlog/spdlog.h"

LoadedImage::LoadedImage(const std::string& filePath)
	: m_filePath(filePath)
{
	spdlog::info("[Opening image] " + m_filePath);
	m_debugName = m_filePath;
	//Load file
	stbi_set_flip_vertically_on_load(1);
	int width, height, BPP;
	unsigned char* pixels = stbi_load(m_filePath.c_str(), &width, &height, &BPP, bytesPerPixel(RGBA));
	if (!pixels)
		spdlog::warn("Couldn't open " + m_filePath);
	Initialize(width, height, BPP, pixels);
	spdlog::info("-------------------------------------------------");
}

LoadedImage::~LoadedImage() {
	spdlog::info("[Image Destructed] " + m_filePath);
	if (m_pixels)
		stbi_image_free(m_pixels);
}