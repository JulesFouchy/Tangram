#include "layer.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "utilities/display.hpp"
#include "utilities/maths.hpp"

#include "graphics/immediateDrawing.hpp"

Layer::Layer(std::string imgFilePath, DrawingBoard* drawingBoard, std::string layerName)
	:	m_image(imgFilePath), m_drawingBoard(drawingBoard), m_name(layerName), m_transform(m_image.getAspectRatio())
{
}

Layer::~Layer() {

}

void Layer::show() {
	float scale = m_transform.getScale();
	glm::vec2 translation = m_transform.getTranslation();
	float dbRatio = m_drawingBoard->transform.getAspectRatio();
	float ratio = m_transform.getAspectRatio();
	m_image.show(m_transform.getMatrix(), m_drawingBoard->transform.getProjectionMatrix());
}

void Layer::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * m_drawingBoard->transform.getMatrix() * m_transform.getMatrix());
	ImmediateDrawing::rectOutline(0.0f, 0.0f, m_transform.getAspectRatio(), 1.0f, 0.002f);
}

void Layer::checkInputs() {
	m_transform.checkInputs(m_drawingBoard->transform.getInverseMatrix());
}

