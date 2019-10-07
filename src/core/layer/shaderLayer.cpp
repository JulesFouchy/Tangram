#include "shaderLayer.hpp"

#include "imgui/imgui.h"

#include "UI/log.hpp"
#include "UI/input.hpp"
#include "UI/settings.hpp"

#include "core/drawingBoard.hpp"

#include <fstream>
#include "helper/string.hpp"

ShaderLayer::ShaderLayer(int previewWidth, int previewHeight, const std::string& fragmentFilePath)
	: Layer((float) previewWidth / previewHeight, fragmentFilePath), m_shader("C:/Dev/Tangram/res/shaders/vertex/shaderLayer.vert", fragmentFilePath),
	  m_uniforms(), m_rectVAO()
{
	parseShader(fragmentFilePath);
	// Initialize members
	m_rectVAO.Initialize(-1.0f, 1.0f, -1.0f, 1.0f, m_transform.getInitialAspectRatio(), MINUS_RATIO_TO_RATIO__MINUS_ONE_TO_ONE);
	m_renderBuffer.getTexture().Initialize(previewWidth, previewHeight, Texture2D::bytesPerPixel(RGBA), nullptr);
	drawShaderOnPreviewTexture();
}

void ShaderLayer::reload() {
	m_shader.compile();
	parseShader(m_shader.getFragmentFilepath());
	drawShaderOnPreviewTexture();
}

void ShaderLayer::pushUniformChangeInHistory(Uniform& uniform) {
	DrawingBoard::history.beginUndoGroup();
	UniformType prevValue = uniform.getValueWhenDraggingStarted();
	UniformType newValue = uniform.getValue();
	DrawingBoard::history.addAction(Action(
		// DO action
		[this, &uniform, newValue]()
	{
		uniform.setValue(newValue);
		drawShaderOnPreviewTexture();
	},
		// UNDO action
		[this, &uniform, prevValue]()
	{
		uniform.setValue(prevValue);
		drawShaderOnPreviewTexture();
	}
	));
	DrawingBoard::history.endUndoGroup();
}

void ShaderLayer::showGUI() {
	ImGui::SetNextWindowSize(ImVec2(280, 280), ImGuiCond_FirstUseEver);
	ImGui::Begin(("Uniforms of " + getName()).c_str());
	bool uniformChanged = false;
	for (Uniform& uniform : m_uniforms) {
		auto [bUniformJustChanged, bDraggingJustEnded] = uniform.GuiDragValue();
		uniformChanged |= bUniformJustChanged;
		if (bDraggingJustEnded) {
			DrawingBoard::history.beginUndoGroup();
			UniformType prevValue = uniform.getValueWhenDraggingStarted();
			UniformType newValue = uniform.getValue();
			DrawingBoard::history.addAction(Action(
				// DO action
				[this, &uniform, newValue]()
			{
				uniform.setValue(newValue);
				drawShaderOnPreviewTexture();
			},
				// UNDO action
				[this, &uniform, prevValue]()
			{
				uniform.setValue(prevValue);
				drawShaderOnPreviewTexture();
			}
			));
			DrawingBoard::history.endUndoGroup();
		}
	}
	if (uniformChanged)
		drawShaderOnPreviewTexture();
	ImGui::End();
}

void ShaderLayer::showDraggablePoints() {
	for (Uniform& uniform : m_uniforms) {
		uniform.showDraggablePoints();
	}
}

DraggablePoint* ShaderLayer::lookForHoveredDraggablePoint() {
	for (Uniform& uniform : m_uniforms) {
		if (auto point = std::get_if<DraggablePoint>(&uniform.getValue())) {
			if (glm::length(point->getPos_WS() - Input::getMousePosition()) < Settings::ALT_ORIGIN_RADIUS)
				return (DraggablePoint*) point;
		}
	}
	return nullptr;
}

void ShaderLayer::showForSaving(RectTransform& transform) {
	shaderBindAndSetFragmentUniforms();
	m_shader.setUniformMat4f("u_mvp", DrawingBoard::transform.getProjectionMatrix() * transform.getMatrix() * glm::inverse(transform.getProjectionMatrix()));
	m_rectVAO.binddrawunbind();
}

void ShaderLayer::shaderBindAndSetFragmentUniforms() {
	m_shader.bind();
	for (Uniform& uniform : m_uniforms)
		uniform.set();
}

void ShaderLayer::drawShaderOnPreviewTexture() {
	spdlog::warn("recomputed shader {}", getName());
	glDisable(GL_BLEND);
		shaderBindAndSetFragmentUniforms();
		m_shader.setUniformMat4f("u_mvp", glm::mat4x4(1.0f));
		m_renderBuffer.bind();
		m_renderBuffer.clear();
		m_rectVAO.binddrawunbind();
		m_renderBuffer.unbind(); 
	glEnable(GL_BLEND);
}

void ShaderLayer::parseShader(const std::string& filepath) {
	std::ifstream file(filepath);
	if (file.is_open()) {
		std::vector<Uniform> tmpUniforms;

		std::string line;
		while (std::getline(file, line)) {
			//spdlog::info("NEW LINE");
			// Parse uniform
			size_t posBeginUniform = line.find("uniform");
			size_t posBeginComment = line.find("//");
			if (posBeginUniform != std::string::npos && posBeginUniform < posBeginComment) {
				// Get type
				size_t posBeginType = String::beginningOfNextWord(line, String::endOfNextWord(line, posBeginUniform) + 1);
				size_t posEndType = String::endOfNextWord(line, posBeginType);
				OpenGLType type = stringToOpenGLType(line.substr(posBeginType, posEndType - posBeginType));
				UniformTypePrecisions uniformTypePrecisions(type);
				// Get name
				size_t posBeginName = String::beginningOfNextWord(line, posEndType);
				size_t posEndName = String::endOfNextWord(line, posBeginName);
				std::string s_name = line.substr(posBeginName, posEndName - posBeginName);
				spdlog::info("found uniform {}", s_name);
				// Get options
				UniformType initialValue = Uniform::zero(uniformTypePrecisions);
				UniformType minValue = Uniform::zero(uniformTypePrecisions);
				UniformType maxValue = Uniform::zero(uniformTypePrecisions);
				if (posBeginComment != std::string::npos) {
					spdlog::info("looking for options");
					size_t currentPos = String::beginningOfNextWord(line, String::endOfNextWord(line, posBeginComment) + 1);
					while (currentPos < line.size()) {
						std::string arg = String::getNextWord(line, &currentPos);
						spdlog::info("|" + arg + "|");
						if (arg == "NOT_A_COLOR" || arg == "NOT_A_COLOUR") {
							uniformTypePrecisions.setShowAsAColorPicker(false);
						}
						else if (arg == "POINT2D") {
							uniformTypePrecisions.setShowAsDraggable2DPoint(true);
						}
						else if (arg == "default") {
							int index = -1;
							for (int k = 0; k < m_uniforms.size(); ++k) {
								if (m_uniforms[k].getName() == s_name) {
									index = k;
									break;
								}
							}
							if (index != -1)
								initialValue = m_uniforms[index].getValue();
							else
								initialValue = readValue_s_(uniformTypePrecisions, line, &currentPos);
						}
						else if (arg == "min") {
							minValue = readValue_s_(uniformTypePrecisions, line, &currentPos);
						}
						else if (arg == "max") {
							maxValue = readValue_s_(uniformTypePrecisions, line, &currentPos);
						}
					}
				}
				// Add uniform
				tmpUniforms.push_back(Uniform(m_shader.getID(), s_name, initialValue, minValue, maxValue, uniformTypePrecisions));
				// special handling of DraggablePoints
				if (uniformTypePrecisions.shouldShowAsADraggable2DPoint()) {
					setMovability(false);
					DraggablePoint& dragPt = std::get<DraggablePoint>(*tmpUniforms[tmpUniforms.size()-1].getValuePointer());
					dragPt.setParentTransform(&m_transform);
					dragPt.setParentShaderLayer(this);
				}
			}
		}
		m_uniforms = tmpUniforms;
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

UniformType ShaderLayer::readValue_s_(const UniformTypePrecisions& typePrecisions, const std::string& str, size_t* currentPosPtr) {
	float x, y, z, w;
	switch (typePrecisions.getOpenGLType())
	{
	case Int:
		return std::stoi(String::getNextWord(str, currentPosPtr));
		break;
	case Float:
		return std::stof(String::getNextWord(str, currentPosPtr));
		break;
	case Vec2:
		x = std::stof(String::getNextWord(str, currentPosPtr));
		y = std::stof(String::getNextWord(str, currentPosPtr));
		if (typePrecisions.shouldShowAsADraggable2DPoint())
			return DraggablePoint(x, y, nullptr);
		else
			return glm::vec2(x, y);
		break;
	case Vec3:
		x = std::stof(String::getNextWord(str, currentPosPtr));
		y = std::stof(String::getNextWord(str, currentPosPtr));
		z = std::stof(String::getNextWord(str, currentPosPtr));
		return glm::vec3(x, y, z);
		break;
	case Vec4:
		x = std::stof(String::getNextWord(str, currentPosPtr));
		y = std::stof(String::getNextWord(str, currentPosPtr));
		z = std::stof(String::getNextWord(str, currentPosPtr));
		w = std::stof(String::getNextWord(str, currentPosPtr));
		return glm::vec4(x, y, z, w);
		break;
	default:
		spdlog::error("[ShaderLayer::readValue_s_] unknown OpenGLType");
		break;
	}
}

void ShaderLayer::createACopy() {
	ShaderLayer* newLayer = new ShaderLayer(getTexture().getWidth(), getTexture().getHeight(), m_shader.getFragmentFilepath());
	newLayer->setTransform(m_transform);
	newLayer->m_uniforms = m_uniforms;
	newLayer->drawShaderOnPreviewTexture();
	DrawingBoard::LayerRegistry().addLayer(newLayer);
}