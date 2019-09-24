#include "shaderLayer.hpp"

#include "imgui/imgui.h"

#include "UI/log.hpp"

#include <fstream>
#include "utilities/string.hpp"

ShaderLayer::ShaderLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath)
	: Layer((float) previewWidth / previewHeight, fragmentFilePath), m_shader("res/shaders/vertex/shaderLayer.vert", fragmentFilePath),
	  m_uniforms(), m_rectVAO()
{
	parseShader(fragmentFilePath);
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

void ShaderLayer::parseShader(const std::string& filepath) {
	std::ifstream file(filepath);
	if (file.is_open()) {
		std::string line;

		while (std::getline(file, line)) {
			size_t posInString = 0U;
			// Parse uniform line
			if ( (posInString = line.find("uniform")) != std::string::npos) {
				size_t posBeginUniform = line.find("uniform");
				// Get type
				size_t posBeginType = posBeginUniform + std::string("uniform").size() + 1;
				size_t posEndType = line.find(' ', posBeginType);
				const std::string s_type = line.substr(posBeginType, posEndType - posBeginType);
				// Get name
				size_t posBeginName = posEndType + 1;
				size_t posEndName = std::min(line.find(' ', posBeginName), line.find(';', posBeginName));
				std::string s_name = line.substr(posBeginName, posEndName - posBeginName);
				// Add uniform
				m_uniforms.push_back(Uniform(m_shader.getID(), s_name, 0.0f));
			}
			//else if (line.find("carte") != std::string::npos) { m_mapPath += line.substr(6, line.size()); }
		}

	}
}

void ShaderLayer::showUI() {
	ImGui::Begin(( "Uniforms of " + getName() ).c_str());
		for (Uniform& uniform : m_uniforms) {
			if (ImGui::SliderFloat(uniform.getName().c_str(), uniform.getValuePointer(), 0.0f, 1.0f))
				drawShaderOnTexture();
		}
	ImGui::End();
}