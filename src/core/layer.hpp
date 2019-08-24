#pragma once

#include "image.hpp"
#include "glm/glm.hpp"

class Layer {
public:
	Layer(std::string imgFilePath, std::string layerName = "New layer");
	~Layer();

	void show(glm::mat4x4 transform, glm::mat4x4 projection);
	void show(glm::mat4x4 transform);

	void checkInputs();

	void setTranslation(glm::vec2 translation);
	void translate(glm::vec2 translation);
	void setScale(float scale);
	void scale(float scale);
	void setRotation(float rotation);
	void rotate(float rotation);

private:
	Image m_image;
	std::string m_name;
private:
	glm::vec2 m_translation;
	float m_scale;
	float m_rotation;
	glm::mat4x4 m_transform;
	void computeTransformMatrix();
};