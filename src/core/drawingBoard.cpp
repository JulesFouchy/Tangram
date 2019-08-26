#include "drawingBoard.hpp"

#include "graphics/immediateDrawing.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "utilities/display.hpp"

#include "UI/input.hpp"
#include "UI/log.hpp"

#include "stb_image/stb_image_write.h"

DrawingBoardTransform DrawingBoard::transform(1.0f);
LayerList DrawingBoard::layers;
FrameBuffer* DrawingBoard::renderBuffer;

void DrawingBoard::Initialize(float whRatio) 
{
	transform = DrawingBoardTransform(whRatio);
	renderBuffer = new FrameBuffer(Display::getHeight() * whRatio, Display::getHeight());
}

void DrawingBoard::show() {
	renderBuffer->bind();
	renderBuffer->clear();
		layers.show();
	renderBuffer->unbind();
	renderBuffer->m_texture.show(transform.getMatrix());

	layers.showFrames();

	showFrame();
}

void DrawingBoard::showForSaving() {
	layers.show();
}

void DrawingBoard::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * transform.getMatrix());
	ImmediateDrawing::rectOutline(0.0f, 0.0f, transform.getAspectRatio(), 1.0f, 0.002f);
}

void DrawingBoard::save(int approxNbPixels, std::string filePath) {
	//Compute output width and height
	float w = sqrt(approxNbPixels * transform.getAspectRatio());
	float h = w / transform.getAspectRatio();
	int width = floor(w);
	int height = floor(h);
	//Bind frameBuffer to render and save
	FrameBuffer saveBuffer(width, height);
	saveBuffer.bind();
	saveBuffer.clear();
	showForSaving();
	//
	unsigned char* data = new unsigned char[4*width*height];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(1);
	stbi_write_png(filePath.c_str(), width, height, 4, data, 0);
	//
	saveBuffer.unbind();
}

void DrawingBoard::addLayer(const std::string imgFilePath) {
	layers.addLayer(imgFilePath);
}

void DrawingBoard::update() {
	transform.checkDraggingTranslation();
}

void DrawingBoard::onDoubleLeftClic() {
	//Center the point we doucle-clicked on
	transform.translate(-Input::getMousePosition());
}

void DrawingBoard::onLeftClicDown() {
	if (Input::spaceBarIsDown() && !layers.isHandlingAnInput()) {
		transform.startDraggingTranslation();
	}
	else{
		layers.onLeftClicDown();
	}
}

void DrawingBoard::onLeftClicUp() {
	if (!transform.endDragging()) {
		layers.onLeftClicUp();
	}
}

void DrawingBoard::onSpaceBarDown() {
	if (Input::leftClicIsDown() && !layers.isHandlingAnInput()) {
		transform.startDraggingTranslation();
	}
}

void DrawingBoard::onSpaceBarUp() {
	transform.endDragging();
}