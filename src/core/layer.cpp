#include "layer.hpp"

#include "drawingBoard.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "utilities/display.hpp"
#include "utilities/maths.hpp"

#include "graphics/immediateDrawing.hpp"

Layer::Layer(std::string imgFilePath, std::string layerName)
	: m_bVisible(true), m_image(imgFilePath), m_name(layerName), m_transform(m_image.getAspectRatio())
{
}

Layer::~Layer() {

}

void Layer::show(glm::mat4x4 viewMatrix, glm::mat4x4 projMatrix) {
	m_image.show(viewMatrix * m_transform.getMatrix(), projMatrix);
}

void Layer::showInDrawingBoardSpace() {
	m_image.show(m_transform.getMatrix(), DrawingBoard::transform.getProjectionMatrix());
}

void Layer::showFrame() {
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 1.0f);
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * DrawingBoard::transform.getMatrix() * m_transform.getMatrix());
	ImmediateDrawing::rectOutline(0.0f, 0.0f, m_transform.getAspectRatio(), 1.0f, 0.002f);
}