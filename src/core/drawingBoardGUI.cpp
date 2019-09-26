#include "drawingBoardGUI.hpp"
#include "drawingBoard.hpp"

#include "imgui/imgui.h"

bool DrawingBoardGUI::m_bShaderLayerCreationWindow = false;


void DrawingBoardGUI::Show() {
	if (m_bShaderLayerCreationWindow)
		ImGuiShaderLayerCreationWindow();
}

void DrawingBoardGUI::OpenCreateShaderLayerWindow() {
	m_bShaderLayerCreationWindow = true;
}

void DrawingBoardGUI::ImGuiShaderLayerCreationWindow() {
	ImGui::Begin("Creating a ShaderLayer");

	ImGui::End();
}