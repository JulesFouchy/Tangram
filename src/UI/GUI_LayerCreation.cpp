#include "GUI_LayerCreation.hpp"
#include "core/drawingBoard.hpp"

#include "UI/fileBrowser.hpp"
#include "helper/string.hpp"

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

bool GUI_LayerCreation::m_bWindow_ShaderLayerCreation = false;
bool GUI_LayerCreation::m_bWindow_DrawingBoardSaving = false;
Ratio GUI_LayerCreation::m_aspectRatio(1, 1);
unsigned int GUI_LayerCreation::m_width = 1000;
unsigned int GUI_LayerCreation::m_height = 1000;
WidthOrHeight GUI_LayerCreation::m_lastModified = Height;

std::string GUI_LayerCreation::m_filepath = "";

void GUI_LayerCreation::Show() {
	if (m_bWindow_ShaderLayerCreation)
		Window_ShaderLayerCreation();
	if (m_bWindow_DrawingBoardSaving)
		Window_DrawingBoardSaving();
}

void GUI_LayerCreation::OpenCreateShaderLayerWindow() {
	if (!isBusy()) { // can only open one window at a time because they share member variables
		m_bWindow_ShaderLayerCreation = true;
		m_aspectRatio = DrawingBoard::transform.getAspectRatio();
		updateWidthOrHeight();
	}
}

void GUI_LayerCreation::OpenSaveDrawingBoardWindow() {
	if (!isBusy()) { // can only open one window at a time because they share member variables
		m_bWindow_DrawingBoardSaving = true;
		m_aspectRatio = DrawingBoard::transform.getAspectRatio();
		updateWidthOrHeight();
		m_filepath = "";
	}
}

void GUI_LayerCreation::Window_ShaderLayerCreation() {
	ImGui::Begin("Creating a ShaderLayer", &m_bWindow_ShaderLayerCreation);
	// get width and height
	ImGuiChoose_Ratio_Width_Height();
	ImGui::Separator();

	// get filepath
	ImGui::Text("Fragment file path : "); ImGui::SameLine();
	ImGuiOpenFileButton("Fragment shader (*.frag; *.fragment)\0*.frag;*.fragment\0All Files (*.*)\0*.*\0");
	ImGui::Separator();

	// creation	
	if (ImGui::Button("OK !")) {
		DrawingBoard::getLayerList().createShaderLayer(m_width, m_height, m_filepath);
		m_bWindow_ShaderLayerCreation = false;
	}
	ImGui::End();
}

void GUI_LayerCreation::Window_DrawingBoardSaving() {

	ImGui::Begin("Saving", &m_bWindow_DrawingBoardSaving);
	// get width and height
	ImGuiChoose_Width_Height();
	ImGui::Separator();

	// get filepath
	ImGui::Text("Save as : "); ImGui::SameLine();
	ImGuiSaveFileNameButton();
	ImGui::Separator();

	// creation	
	if (ImGui::Button("OK !")) {
		DrawingBoard::save(m_width, m_height, m_filepath);
		m_bWindow_DrawingBoardSaving = false;
	}
	ImGui::End();
}

void GUI_LayerCreation::updateWidthOrHeight() {
	switch (m_lastModified) {
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
void GUI_LayerCreation::ImGuiChoose_Width_Height() {
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
	ImGui::PopID();
	ImGui::PopItemWidth();
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
	if (ImGui::Button("Square")) {
		m_aspectRatio.set(1,1);
		updateWidthOrHeight();
	}
	ImGui::SameLine();
	if (ImGui::Button("Same as DrawingBoard's")) {
		m_aspectRatio = DrawingBoard::transform.getAspectRatio();
		updateWidthOrHeight();
	}
	ImGui::PopItemWidth();
	// Width and Height
	ImGuiChoose_Width_Height();
}

void GUI_LayerCreation::ImGuiOpenFileButton(const char* filter) {
	ImGui::InputText("", &m_filepath);
	if (ImGui::Button("Choose file")) {
		m_filepath = openfilename(filter);
	}
}

void GUI_LayerCreation::ImGuiSaveFileNameButton() {
	ImGui::InputText("", &m_filepath);
	if (ImGui::Button("Choose destination")) {
		m_filepath = savefilename();
	}
}