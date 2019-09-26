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

	void showGUI() override;
private:
	void drawShaderOnTexture();

private:
	Shader m_shader;
	std::vector<Uniform> m_uniforms;
	RectVAO m_rectVAO;

private:
	// Parsing
	void parseShader(const std::string& filepath);
	static OpenGLType stringToOpenGLType(const std::string& s_type);
	static UniformType readValue_s_(OpenGLType type, const std::string& str, size_t* currentPosPtr);
};