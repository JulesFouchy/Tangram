#pragma once

#include "glm/glm.hpp"

#include "graphics/loadedImage.hpp"
#include "transforms/rectTransform.hpp"

class Layer {
public:
	Layer(std::string imgFilePath, std::string layerName = "New layer");
	~Layer();

	void show();
	void showFrame();

	void checkInputs();

private:
	LoadedImage m_image;
	std::string m_name;
public:
	RectTransform m_transform;
};