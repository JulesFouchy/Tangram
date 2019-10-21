#include "EffectLayerCreation.hpp"

#include "core/drawingBoard.hpp"

#include "imgui/imgui.h"

#include "UI/log.hpp"

GUIwindow_EffectLayerCreation::GUIwindow_EffectLayerCreation()
	: GUIwindow("Creating an EffectLayer", [this]() {
		DrawingBoard::LayerRegistry().createEffectLayer(m_WidthHeightRatioPicker.getRatio(), m_WidthHeightRatioPicker.getHeight(), m_filepathPicker.getFilepath(), m_targetLayerID);
	  }),
	  m_targetLayerID(0),
	  m_WidthHeightRatioPicker(),
	  m_filepathPicker("Fragment shader (*.frag; *.fragment)\0*.frag;*.fragment\0All Files (*.*)\0*.*\0")
{
}

void GUIwindow_EffectLayerCreation::Open(LayerID targetLayerID) {
	GUIwindow::Open();
	m_targetLayerID = targetLayerID;
	m_WidthHeightRatioPicker.setRatio(DrawingBoard::LayerRegistry()[targetLayerID]->m_transform.getAspectRatio());
}

void GUIwindow_EffectLayerCreation::Show() {
	BeginWindow();
	//
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