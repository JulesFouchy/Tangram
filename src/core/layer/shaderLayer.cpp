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

void ShaderLayer::showUI() {
	ImGui::Begin(("Uniforms of " + getName()).c_str());
	for (Uniform& uniform : m_uniforms) {
		if (uniform.ImGuiDragValue())
			drawShaderOnTexture();
	}
	ImGui::End();
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
				OpenGLType type = stringToOpenGLType(line.substr(posBeginType, posEndType - posBeginType));
				// Get name
				size_t posBeginName = ParseShader::beginningOfNextWord(line, posEndType);
				size_t posEndName = ParseShader::endOfNextWord(line, posBeginName);
				std::string s_name = line.substr(posBeginName, posEndName - posBeginName);
				spdlog::info("found uniform {}", s_name);
				// Get options
				UniformType initialValue = Uniform::zero(type);
				UniformType minValue = Uniform::zero(type);
				UniformType maxValue = Uniform::one(type);
				if (posBeginComment != std::string::npos) {
					spdlog::info("looking for options");
					size_t currentPos = ParseShader::beginningOfNextWord(line, ParseShader::endOfNextWord(line, posBeginComment) + 1);
					while (currentPos < line.size()) {
						std::string arg = ParseShader::getNextWord(line, &currentPos);
						spdlog::info("|" + arg + "|");
						if (arg == "default") {
							initialValue = readValue_s_(type, line, &currentPos);
						}
						else if (arg == "min") {
							minValue = readValue_s_(type, line, &currentPos);
						}
						else if (arg == "max") {
							maxValue = readValue_s_(type, line, &currentPos);
						}
					}
				}
				// Add uniform
				m_uniforms.push_back(Uniform(m_shader.getID(), s_name, initialValue, minValue, maxValue));
			}
		}

	}
}

OpenGLType ShaderLayer::stringToOpenGLType(const std::string& s_type) {
	if (s_type == "int") {
		return Int;
	}
	else if (s_type == "float") {
		return Float;
	}
	else if (s_type == "vec2") {
		return Vec2;
	}
	else if (s_type == "vec3") {
		return Vec3;
	}
	else if (s_type == "vec4") {
		return Vec4;
	}
	else {
		spdlog::error("[stringToOpenGLType] unknown type : {}", s_type);
	}
}

UniformType ShaderLayer::readValue_s_(OpenGLType type, const std::string& str, size_t* currentPosPtr) {
	float x, y, z, w;
	switch (type)
	{
	case Int:
		return std::stoi(ParseShader::getNextWord(str, currentPosPtr));
		break;
	case Float:
		return std::stof(ParseShader::getNextWord(str, currentPosPtr));
		break;
	case Vec2:
		x = std::stof(ParseShader::getNextWord(str, currentPosPtr));
		y = std::stof(ParseShader::getNextWord(str, currentPosPtr));
		return glm::vec2(x, y);
		break;
	case Vec3:
		x = std::stof(ParseShader::getNextWord(str, currentPosPtr));
		y = std::stof(ParseShader::getNextWord(str, currentPosPtr));
		z = std::stof(ParseShader::getNextWord(str, currentPosPtr));
		return glm::vec3(x, y, z);
		break;
	case Vec4:
		x = std::stof(ParseShader::getNextWord(str, currentPosPtr));
		y = std::stof(ParseShader::getNextWord(str, currentPosPtr));
		z = std::stof(ParseShader::getNextWord(str, currentPosPtr));
		w = std::stof(ParseShader::getNextWord(str, currentPosPtr));
		return glm::vec4(x, y, z, w);
		break;
	default:
		spdlog::error("[ShaderLayer::readValue_s_] unknown OpenGLType");
		break;
	}
}