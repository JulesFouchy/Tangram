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
	Log::separationLine();
	// Initialize members
	m_bMustRecomputeSaveBuffer = false; // permanently false for a loaded image layer
	m_previewBuffer.getTexture().Initialize(width, height, BPP, pixels); // TODO if image is too big, only put low-res texture here for the sake of previex speed
	m_saveBuffer.getTexture().Initialize(width, height, BPP, pixels);
	m_transform = RectTransform((float) width / height);
	// Free pixels CPU-side
	if (pixels)
		stbi_image_free(pixels);
	else
		spdlog::warn("Couldn't open " + imgFilePath);
}

LoadedImageLayer::~LoadedImageLayer() {
	spdlog::info("[Loaded Image Layer Destructed] " + getName());
}

void LoadedImageLayer::createACopy() {
	Layer* newLayer = new LoadedImageLayer(m_filePath);
	newLayer->setTransform(m_transform);
	DrawingBoard::LayerRegistry().addLayer(newLayer);
}

void LoadedImageLayer::drawOnFrameBuffer_Preview(FrameBuffer& frameBuffer) {
	drawOnFrameBuffer_Save(frameBuffer, 0);
}
void LoadedImageLayer::drawOnFrameBuffer_Save(FrameBuffer& frameBuffer, int drawingBoardHeight) {
	spdlog::warn("recomputed shader {}", getName());
	glDisable(GL_BLEND);
		frameBuffer.bind();
		frameBuffer.clear();
		getTexture_Save().show(glm::mat4x4(1.0f));
		frameBuffer.unbind();
	glEnable(GL_BLEND);
}