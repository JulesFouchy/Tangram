#include "uniform.hpp"
#include <variant>

#include "imgui/imgui.h"

#include "glm/gtc/type_ptr.hpp"

#include "UI/log.hpp"

Uniform::Uniform(GLuint shaderID, const std::string name, UniformType value, UniformType minValue, UniformType maxValue)
	: m_name     ( name ),
	  m_location ( glGetUniformLocation(shaderID, name.c_str()) ),
	  m_value    (value),
	  m_minValue (minValue),
	  m_maxValue (maxValue)
{
}

void Uniform::set() {
	if (auto myInt = std::get_if<int>(getValuePointer())) {
		glUniform1i(getLocation(), *myInt);
	}
	else if (auto myFloat = std::get_if<float>(getValuePointer())) {
		glUniform1f(getLocation(), *myFloat);
	}
	else if (auto myVec2 = std::get_if<glm::vec2>(getValuePointer())) {
		glUniform2f(getLocation(), myVec2->x, myVec2->y);
	}
	else if (auto myDraggablePoint = std::get_if<DraggablePoint>(getValuePointer())) {
		const glm::vec2& pos = myDraggablePoint->getPos_TS();
		glUniform2f(getLocation(), pos.x, pos.y);
	}
	else if (auto myVec3 = std::get_if<glm::vec3>(getValuePointer())) {
		glUniform3f(getLocation(), myVec3->x, myVec3->y, myVec3->z);
	}
	else if (auto myVec4 = std::get_if<glm::vec4>(getValuePointer())) {
		glUniform4f(getLocation(), myVec4->x, myVec4->y, myVec4->z, myVec4->w);
	}
	else {
		spdlog::error(" {} : unknown uniform type", getName());
	}
}

bool Uniform::GuiDragValue() {
	if (auto myInt = std::get_if<int>(getValuePointer())) {
		return ImGui::DragInt(getName().c_str(), myInt);
	}
	else if (auto myFloat = std::get_if<float>(getValuePointer())) {
		return ImGui::SliderFloat(getName().c_str(), myFloat, std::get<float>(m_minValue), std::get<float>(m_maxValue));
	}
	else if (auto myVec2 = std::get_if<glm::vec2>(getValuePointer())) {
		//return TanGUI::DragPoint(m_pos2D_WS);
		return ImGui::SliderFloat2(getName().c_str(), glm::value_ptr(*myVec2), std::get<glm::vec2>(m_minValue).x, std::get<glm::vec2>(m_maxValue).x);
	}
	else if (auto myDraggablePoint = std::get_if<DraggablePoint>(getValuePointer())) {
		//return TanGUI::DragPoint(m_pos2D_WS);
		//return ImGui::SliderFloat2(getName().c_str(), glm::value_ptr(*myVec2), std::get<glm::vec2>(m_minValue).x, std::get<glm::vec2>(m_maxValue).x);
	}
	else if (auto myVec3 = std::get_if<glm::vec3>(getValuePointer())) {
		return ImGui::ColorPicker3(getName().c_str(), glm::value_ptr(*myVec3));
	}
	else if (auto myVec4 = std::get_if<glm::vec4>(getValuePointer())) {
		return ImGui::ColorPicker4(getName().c_str(), glm::value_ptr(*myVec4));
	}
	else {
		spdlog::error(" {} : unknown uniform type", getName());
	}
}

void Uniform::showDraggablePoints() {
	if (auto point = std::get_if<DraggablePoint>(getValuePointer())) {
		point->show();
		//TanGUI::DragPoint(m_pos2D_WS);
		//return ImGui::SliderFloat2(getName().c_str(), glm::value_ptr(*myVec2), std::get<glm::vec2>(m_minValue).x, std::get<glm::vec2>(m_maxValue).x);
	}
}

UniformType Uniform::zero(OpenGLType type) {
	switch (type)
	{
	case Int:
		return 0;
		break;
	case Float:
		return 0.0f;
		break;
	case Vec2:
		return DraggablePoint(0.0f, 0.0f, nullptr);
		break;
	case Vec3:
		return glm::vec3(0.0f);
		break;
	case Vec4:
		return glm::vec4(0.0f);
		break;
	default:
		spdlog::error("[Uniform::zero] unknow OpenGLType");
		break;
	}
}

UniformType Uniform::one(OpenGLType type) {
	switch (type)
	{
	case Int:
		return 1;
		break;
	case Float:
		return 1.0f;
		break;
	case Vec2:
		return DraggablePoint(1.0f, 1.0f, nullptr);
		break;
	case Vec3:
		return glm::vec3(1.0f);
		break;
	case Vec4:
		return glm::vec4(1.0f);
		break;
	default:
		spdlog::error("[Uniform::one] unknow OpenGLType");
		break;
	}
}