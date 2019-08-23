#pragma once

#include "image.hpp"
#include "glm/glm.hpp"

class Layer {
public:
	Layer(std::string imgFilePath, std::string layerName = "New layer");
	~Layer();
	void show(glm::vec2 translation = glm::vec2(0.0f), float scale = 1.0f, float rotation = 0.0f);

private:
	Image m_image;
	std::string m_name;
private:
	glm::vec2 m_translation;
	float m_scale;
	float m_rotation;
};