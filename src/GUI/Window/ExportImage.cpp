#include "ExportImage.hpp"

#include "core/drawingBoard.hpp"

#include "imgui/imgui.h"

#include "UI/settings.hpp"

#include "UI/log.hpp"

GUIwindow_ExportImage::GUIwindow_ExportImage()
	: GUIwindow("Exporting Image", [this]() {
		DrawingBoard::save(m_WidthHeightRatioPicker.getHeight(), m_FilepathPicker.getFilepath());
	}),
	m_WidthHeightRatioPicker(),
	m_FilepathPicker(" PNG\0*.png;*.PNG\0 JPEG\0*.jpg;*.jpeg;*.JPG;*.JPEG\0")
{

}

void GUIwindow_ExportImage::Open() {
	GUIwindow::Open();
	m_FilepathPicker.setFilepath(Settings::EXECUTABLE_FILEPATH() + "data/out/test.png");
	m_WidthHeightRatioPicker.setRatio(DrawingBoard::transform.getAspectRatio());
}

void GUIwindow_ExportImage::Show() {
	BeginWindow();
	//
	m_WidthHeightRatioPicker.ShowWidthHeight();
	ImGui::Separator();
	//
	ImGui::Text("Save as : "); ImGui::SameLine();
	m_FilepathPicker.ShowSavefilename();
	//
	ImGui::Separator();
	ConfirmationButton();
	EndWindow();
}