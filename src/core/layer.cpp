#include "layer.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "utilities/display.hpp"

#include "graphics/immediateDrawing.hpp"

Layer::Layer(std::string imgFilePath, std::string layerName)
	:	m_image(imgFilePath), m_name(layerName), m_transform(m_image.getAspectRatio())
{
}

Layer::~Layer() {

}

void Layer::show(glm::mat4x4 viewTransform, glm::mat4x4 projection) {
	m_image.show(viewTransform * m_transform.getMatrix(), projection);
}

void Layer::show(glm::mat4x4 viewTransform) {
	show(viewTransform, Display::getProjMat());
}

void Layer::checkInputs() {
	m_transform.checkInputs();
}

