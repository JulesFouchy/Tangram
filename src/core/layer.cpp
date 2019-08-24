#include "layer.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "utilities/display.hpp"

Layer::Layer(std::string imgFilePath, std::string layerName)
	:	m_image(imgFilePath), m_name(layerName), 
		m_translation(glm::vec2(0.0f)), m_scale(1.0f), m_rotation(0.0f), m_transform(glm::mat4x4(1.0f))
{

}

Layer::~Layer() {

}

void Layer::show(glm::mat4x4 transform, glm::mat4x4 projection) {
	m_image.show( transform * m_transform, projection);
}

void Layer::show(glm::mat4x4 transform) {
	show(transform, Display::getProjMat());
}

void Layer::checkInputs() {

}

void Layer::setTranslation(glm::vec2 translation) {
	m_translation = translation;
	computeTransformMatrix();
}
void Layer::translate(glm::vec2 translation) {
	setTranslation(m_translation + translation);
}
void Layer::setScale(float scale) {
	m_scale = scale;
	computeTransformMatrix();
}
void Layer::scale(float scale) {
	setScale(m_scale * scale);
}
void Layer::setRotation(float rotation) {
	m_rotation = rotation;
	computeTransformMatrix();
}
void Layer::rotate(float rotation) {
	setRotation(m_rotation + rotation);
}

void Layer::computeTransformMatrix() {
	m_transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(m_translation,0.0f));
	m_transform = glm::rotate(m_transform, m_rotation, glm::vec3(0.0, 0.0, 1.0));
	m_transform = glm::scale( m_transform, glm::vec3(m_scale, m_scale, 1.0f));
}