#pragma once

#include "layer.hpp"

#include "graphics/shader.hpp"
#include "graphics/uniform.hpp"
#include "graphics/rectVAO.hpp"

#include <vector>

class ShaderLayer : public Layer {
public:
	ShaderLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath);
	~ShaderLayer() = default;

	void showGUI() override;
	void showDraggablePoints() override;

	inline bool createdSuccessfully() override { return m_shader.createdSuccessfully(); }
	void reload() override;
	void createACopy() override;

	void showForSaving(RectTransform& transform) override;
private:
	void shaderBindAndSetFragmentUniforms();
	void drawShaderOnPreviewTexture();

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