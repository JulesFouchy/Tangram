#include "GUI_LayerCreation.hpp"
#include "core/drawingBoard.hpp"

#include "imgui/imgui.h"

bool GUI_LayerCreation::m_bShaderLayerCreationWindow = false;


void GUI_LayerCreation::Show() {
	if (m_bShaderLayerCreationWindow)
		ImGuiShaderLayerCreationWindow();
}

void GUI_LayerCreation::OpenCreateShaderLayerWindow() {
	m_bShaderLayerCreationWindow = true;
}

void GUI_LayerCreation::ImGuiShaderLayerCreationWindow() {
	ImGui::Begin("Creating a ShaderLayer");

	ImGui::End();
}