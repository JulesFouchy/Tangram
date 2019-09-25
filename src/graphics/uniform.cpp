#include "uniform.hpp"
#include <variant>

#include "imgui/imgui.h"

#include "glm/gtc/type_ptr.hpp"

#include "UI/log.hpp"

Uniform::Uniform(GLuint shaderID, const std::string name, UniformType value)
	: m_name     ( name ),
	  m_location ( glGetUniformLocation(shaderID, name.c_str()) ),
	  m_value    ( value )
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

bool Uniform::ImGuiDragValue() {
	if (auto myInt = std::get_if<int>(getValuePointer())) {
		return ImGui::DragInt(getName().c_str(), myInt);
	}
	else if (auto myFloat = std::get_if<float>(getValuePointer())) {
		return ImGui::SliderFloat(getName().c_str(), myFloat, 0.0f, 1.0f);
	}
	else if (auto myVec2 = std::get_if<glm::vec2>(getValuePointer())) {
		return ImGui::SliderFloat2(getName().c_str(), glm::value_ptr(*myVec2), 0.0f, 1.0f);
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