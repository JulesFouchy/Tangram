#pragma once

#include "layer.hpp"

#include "graphics/shader.hpp"

class ShaderLayer : public Layer {
public:
	ShaderLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath);
	~ShaderLayer();
private:
	void drawShaderOnTexture();
private:
	Shader m_shader;
};