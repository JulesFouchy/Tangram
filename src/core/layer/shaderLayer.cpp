#include "shaderLayer.hpp"

#include "imgui/imgui.h"

#include "UI/log.hpp"

#include <fstream>
#include "utilities/parseShader.hpp"

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
		uniform.set();
	m_renderBuffer.bind();
	m_renderBuffer.clear();
	m_rectVAO.binddrawunbind();
	m_renderBuffer.unbind();
}

void ShaderLayer::parseShader(const std::string& filepath) {
	std::ifstream file(filepath);
	if (file.is_open()) {
		std::string line;

		while (std::getline(file, line)) {
			spdlog::info("NEW LINE");
			// Parse uniform
			size_t posBeginUniform = line.find("uniform");
			size_t posBeginComment = line.find("//");
			if (posBeginUniform != std::string::npos && posBeginUniform < posBeginComment) {
				// Get type
				size_t posBeginType = ParseShader::beginningOfNextWord(line, ParseShader::endOfNextWord(line, posBeginUniform) + 1);
				size_t posEndType = ParseShader::endOfNextWord(line, posBeginType);
				const std::string s_type = line.substr(posBeginType, posEndType - posBeginType);
				// Get name
				size_t posBeginName = ParseShader::beginningOfNextWord(line, posEndType);
				size_t posEndName = ParseShader::endOfNextWord(line, posBeginName);
				std::string s_name = line.substr(posBeginName, posEndName - posBeginName);
				spdlog::info("found uniform {}", s_name);
				// Get options
				UniformType initialValue;
				if (posBeginComment != std::string::npos) {
					spdlog::info("looking for options");
					size_t beginningWord = ParseShader::beginningOfNextWord(line, ParseShader::endOfNextWord(line, posBeginComment) + 1);
					while (beginningWord < line.size()) {
						size_t endWord = ParseShader::endOfNextWord(line, beginningWord);
						std::string arg = line.substr(beginningWord, endWord - beginningWord);
						spdlog::info("|" + arg + "|");
						if (arg == "default") {
							size_t beginValue = ParseShader::beginningOfNextWord(line, endWord);
							endWord = ParseShader::endOfNextWord(line, beginValue);
							initialValue = std::stof(line.substr(beginValue, endWord - beginValue));
						}
						beginningWord = ParseShader::beginningOfNextWord(line,endWord);
						
					}
				}
				// Add uniform
				if (s_type == "int") {
					m_uniforms.push_back(Uniform(m_shader.getID(), s_name, 0));
				}
				else if (s_type == "float") {
					m_uniforms.push_back(Uniform(m_shader.getID(), s_name, initialValue));
				}
				else if (s_type == "vec2") {
					m_uniforms.push_back(Uniform(m_shader.getID(), s_name, glm::vec2(0.0f)));
				}
				else if (s_type == "vec3") {
					m_uniforms.push_back(Uniform(m_shader.getID(), s_name, glm::vec3(0.0f)));
				}
				else if (s_type == "vec4") {
					m_uniforms.push_back(Uniform(m_shader.getID(), s_name, glm::vec4(0.0f)));
				}
				else {
					spdlog::error("unsupported uniform type : {} ; in shaderLayer {}", s_type, getName());
				}
			}
		}

	}
}

void ShaderLayer::showUI() {
	ImGui::Begin(( "Uniforms of " + getName() ).c_str());
		for (Uniform& uniform : m_uniforms) {
			if (uniform.ImGuiDragValue())
				drawShaderOnTexture();
		}
	ImGui::End();
}