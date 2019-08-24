#pragma once

#include "image.hpp"
#include "glm/glm.hpp"

#include "transforms/rectTransform.hpp"

class Layer {
public:
	Layer(std::string imgFilePath, std::string layerName = "New layer");
	~Layer();

	void show(glm::mat4x4 viewTransform, glm::mat4x4 projection);
	void show(glm::mat4x4 viewTransform);

	void checkInputs();

private:
	Image m_image;
	std::string m_name;
public:
	RectTransform m_transform;
};