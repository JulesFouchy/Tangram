#include "GUI_LayerCreation.hpp"
#include "core/drawingBoard.hpp"

#include "imgui/imgui.h"

bool GUI_LayerCreation::m_bWindow_ShaderLayerCreation = true;
Ratio GUI_LayerCreation::m_aspectRatio(1, 1);
unsigned int GUI_LayerCreation::m_width = 1000;
unsigned int GUI_LayerCreation::m_height = 1000;
WidthOrHeight GUI_LayerCreation::m_lastModified = Height;

void GUI_LayerCreation::Show() {
	if (m_bWindow_ShaderLayerCreation)
		Window_ShaderLayerCreation();
}

void GUI_LayerCreation::OpenCreateShaderLayerWindow() {
	m_bWindow_ShaderLayerCreation = true;
}

void GUI_LayerCreation::Window_ShaderLayerCreation() {
	ImGui::Begin("Creating a ShaderLayer", &m_bWindow_ShaderLayerCreation);
	ImGuiChoose_Ratio_Width_Height();
	ImGui::End();
}

void GUI_LayerCreation::ImGuiChoose_Ratio_Width_Height() {
	ImGui::PushItemWidth(25.f);
	// Aspect ratio
	ImGui::Text("Aspect Ratio : "); ImGui::SameLine();
	ImGui::PushID(0);
	if (ImGui::InputScalar("", ImGuiDataType_U32, m_aspectRatio.getNumeratorPtr(), NULL, NULL, "%u")) {
		updateWidthOrHeight();
	}
	ImGui::SameLine();
	ImGui::PopID();
	ImGui::Text("/"); ImGui::SameLine();
	ImGui::PushID(1);
	if (ImGui::InputScalar("", ImGuiDataType_U32, m_aspectRatio.getDenominatorPtr(), NULL, NULL, "%u")) {
		updateWidthOrHeight();
	}
	ImGui::PopID();
	ImGui::SameLine();
	if (ImGui::Button("Same as DrawingBoard's")) {
		m_aspectRatio = DrawingBoard::transform.getAspectRatio();
		updateWidthOrHeight();
	}
	ImGui::PopItemWidth();
	// Width and Height
	ImGui::PushItemWidth(100.f);
	ImGui::Text("Width : "); ImGui::SameLine();
	ImGui::PushID(2);
	if (ImGui::InputScalar("", ImGuiDataType_U32, &m_width, NULL, NULL, "%u")) {
		m_lastModified = Width;
		updateWidthOrHeight();
	}
	ImGui::SameLine();
	ImGui::PopID();
	ImGui::Text("Height : "); ImGui::SameLine();
	ImGui::PushID(3);
	if (ImGui::InputScalar("", ImGuiDataType_U32, &m_height, NULL, NULL, "%u")) {
		m_lastModified = Height;
		updateWidthOrHeight();
	}
	ImGui::SameLine();
	ImGui::PopID();
	ImGui::PopItemWidth();
}

void GUI_LayerCreation::updateWidthOrHeight() {
	switch (m_lastModified){
	case Width:
		m_height = m_width / m_aspectRatio;
		break;
	case Height:
		m_width = m_height * m_aspectRatio;
		break;
	default:
		break;
	}
}