#include "ShaderLayerCreation.hpp"

#include "core/drawingBoard.hpp"

#include "imgui/imgui.h"

#include "UI/log.hpp"

GUIwindow_ShaderLayerCreation::GUIwindow_ShaderLayerCreation()
	: GUIwindow("Creating a ShaderLayer", [this]() {
			DrawingBoard::LayerRegistry().createShaderLayer(m_WidthHeightRatioPicker.getWidth(), m_WidthHeightRatioPicker.getHeight(), m_filepathPicker.getFilepath());
	  }),
	  m_WidthHeightRatioPicker(),
	  m_filepathPicker("Fragment shader (*.frag; *.fragment)\0*.frag;*.fragment\0All Files (*.*)\0*.*\0")
{

}

void GUIwindow_ShaderLayerCreation::Open() {
	GUIwindow::Open();
	m_WidthHeightRatioPicker.setRatio(DrawingBoard::transform.getAspectRatio());
	m_WidthHeightRatioPicker.updateWidthOrHeight();
}

void GUIwindow_ShaderLayerCreation::Show() {
	BeginWindow();
		//
		m_WidthHeightRatioPicker.ShowRatio();
		m_WidthHeightRatioPicker.ShowWidthHeight();
		ImGui::Separator();
		//
		ImGui::Text("Fragment file path : "); ImGui::SameLine();
		m_filepathPicker.ShowOpenfilename();
		//
		ImGui::Separator();
	ConfirmationButton();
	EndWindow();
}