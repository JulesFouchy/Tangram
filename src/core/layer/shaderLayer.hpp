#pragma once

#include "layer.hpp"

#include "graphics/shader.hpp"
#include "graphics/uniform.hpp"
#include "graphics/rectVAO.hpp"

#include <vector>

class ShaderLayer : public Layer {
public:
	ShaderLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath);
	~ShaderLayer();

	void showUI() override;
private:
	void drawShaderOnTexture();

	void parseShader(const std::string& filepath);
private:
	Shader m_shader;
	std::vector<Uniform> m_uniforms;
	RectVAO m_rectVAO;
};