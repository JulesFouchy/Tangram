#include "shaderLayer.hpp"

#include "imgui/imgui.h"

#include "UI/log.hpp"

ShaderLayer::ShaderLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath)
	: Layer((float) previewWidth / previewHeight, fragmentFilePath), m_shader("res/shaders/vertex/shaderLayer.vert", fragmentFilePath),
	  m_uniforms(), m_rectVAO()
{
	// REMOVE ME
	m_uniforms.push_back(Uniform(m_shader.getID(), "u_Radius", 0.9f));
	// Initialize members
	m_rectVAO.Initialize(-1.0f, 1.0f, -1.0f, 1.0f, m_transform.getAspectRatio(), MINUS_RATIO_TO_RATIO__MINUS_ONE_TO_ONE);
	m_renderBuffer.getTexture().Initialize(previewWidth, previewHeight, Texture2D::bytesPerPixel(RGBA), nullptr);
	drawShaderOnTexture();
}

ShaderLayer::~ShaderLayer() {

}

void ShaderLayer::drawShaderOnTexture() {
	m_shader.bind();
	for (Uniform& uniform : m_uniforms)
		m_shader.setUniform(uniform);
	m_renderBuffer.bind();
	m_rectVAO.binddrawunbind();
	m_renderBuffer.unbind();
}

void ShaderLayer::showUI() {
	ImGui::Begin(( "Uniforms of " + getName() ).c_str());
		for (Uniform& uniform : m_uniforms) {
			if (ImGui::SliderFloat(uniform.getName().c_str(), uniform.getValuePointer(), 0.0f, 1.0f))
				drawShaderOnTexture();
		}
	ImGui::End();
}