#include "layer.hpp"

Layer::Layer(std::string imgFilePath, std::string layerName)
	:	m_image(imgFilePath), m_name(layerName), 
		m_translation(glm::vec2(0.0f,0.5f)), m_scale(1.0f), m_rotation(0.0f)
{

}

Layer::~Layer() {

}

void Layer::show(glm::vec2 translation, float scale, float rotation) {
	m_image.show(translation + m_translation, scale * m_scale, rotation + m_rotation);
}