#include "loadedImageLayer.hpp"

#include "stb_image/stb_image.h"

#include "core/drawingBoard.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "helper/string.hpp"

LoadedImageLayer::LoadedImageLayer(const std::string& imgFilePath)
	: Layer(Ratio(1,1), String::RemoveFolderHierarchy(imgFilePath)), m_filePath(imgFilePath)
	 //      � dummy initialization of Layer's aspectRatio because we need to read image to know it
{
	spdlog::info("[Opening image] " + imgFilePath);
	// Load file 
	stbi_set_flip_vertically_on_load(1);
	int width, height, BPP;
	unsigned char* pixels = stbi_load(imgFilePath.c_str(), &width, &height, &BPP, Texture2D::bytesPerPixel(RGBA));
	Log::separationLine();
	// Initialize members
	//m_bMustRecomputeSaveBuffer = false; // permanently false for a loaded image layer
	m_fullImageData.Initialize(width, height, BPP, pixels);
	m_previewBuffer.setTextureSize(width, height);
	onChange();
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
		float aspectRatio = frameBuffer.getTexture().getAspectRatio();
		m_fullImageData.show(glm::mat4x4(1.0f), glm::ortho(-0.5f * aspectRatio, 0.5f * aspectRatio, -0.5f, 0.5f));
		frameBuffer.unbind();
	glEnable(GL_BLEND);
}