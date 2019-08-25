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

void Layer::show(glm::mat4x4 projection) {
	float scale = m_transform.getScale();
	glm::vec2 translation = m_transform.getTranslation();
	float dbRatio = m_drawingBoard->transform.getAspectRatio();
	float ratio = m_transform.getAspectRatio();
	mX = Maths::clamp( 0.5 + (-0.5*dbRatio - translation.x) / ratio, 0.0f, 1.0f) ;
	MX = Maths::clamp( 0.5 + ( 0.5 * dbRatio - translation.x) / ratio, 0.0f, 1.0f);
	mY = Maths::clamp( 0.5f - 0.5f/scale - translation.y/scale, 0.0f, 1.0f);
	MY = Maths::clamp( 0.5f + 0.5f / scale - translation.y / scale, 0.0f, 1.0f);
	m_image.show(m_drawingBoard->transform.getMatrix() * m_transform.getMatrix(), projection, mX, MX, mY, MY);
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

