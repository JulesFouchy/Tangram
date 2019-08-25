#include "layer.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "utilities/display.hpp"

#include "graphics/immediateDrawing.hpp"

Layer::Layer(std::string imgFilePath, DrawingBoard* drawingBoard, std::string layerName)
	:	m_image(imgFilePath), m_drawingBoard(drawingBoard), m_name(layerName), m_transform(m_image.getAspectRatio())
{
}

Layer::~Layer() {

}

void Layer::show(glm::mat4x4 projection) {
	m_image.show(m_drawingBoard->transform.getMatrix() * m_transform.getMatrix(), projection, 0.5f, 1.0f, 0.5f, 1.0f);
	//frame
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * m_drawingBoard->transform.getMatrix() * m_transform.getMatrix());
	ImmediateDrawing::rectOutline(0.0f, 0.0f, m_transform.getAspectRatio(), 1.0f, 0.002f);
}

void Layer::show() {
	show(Display::getProjMat());
}

void Layer::checkInputs() {
	m_transform.checkInputs(m_drawingBoard->transform.getInverseMatrix());
}

