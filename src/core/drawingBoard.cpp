#include "drawingBoard.hpp"

#include "graphics/immediateDrawing.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "utilities/display.hpp"

#include "spdlog/spdlog.h"


#include "stb_image/stb_image_write.h"

DrawingBoard::DrawingBoard(float whRatio) 
	: transform(whRatio), m_activLayerIndex(-1)
{

}

DrawingBoard::~DrawingBoard() {

}

void DrawingBoard::show() {
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->show();
	}
	showFrame();
}

void DrawingBoard::showForSaving() {
	for (int k = 0; k < layers.size(); ++k) {
		layers[k]->show(glm::ortho(-0.5f * transform.getAspectRatio(), 0.5f * transform.getAspectRatio(), -0.5f, 0.5f));
	}
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	transform.reset();
	glViewport(0, 0, width, height);
	showForSaving();
	//
	unsigned char* data = new unsigned char[4*width*height];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(1);
	stbi_write_png(filePath.c_str(), width, height, 4, data, 0);
	//
	saveBuffer.unbind();
}



void DrawingBoard::addLayer(std::string imgFilePath) {
	layers.push_back(new Layer(imgFilePath, this));
	setActivLayer(layers.size() - 1);
}

Layer* DrawingBoard::getActivLayer() {
	return layers[m_activLayerIndex];
}
void DrawingBoard::setActivLayer(int layerIndex) {
	m_activLayerIndex = layerIndex;
}