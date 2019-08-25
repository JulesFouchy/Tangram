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
	m_image.show(m_drawingBoard->transform.getMatrix() * m_transform.getMatrix(), projection);
}

void Layer::show() {
	show(Display::getProjMat());
}

void Layer::checkInputs(glm::mat4x4 viewMatrix) {
	m_transform.checkInputs(viewMatrix);
}

