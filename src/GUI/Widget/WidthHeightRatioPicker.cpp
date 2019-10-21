#include "WidthHeightRatioPicker.hpp"

#include "imgui/imgui.h"

GUIwidget_WidthHeightRatioPicker::GUIwidget_WidthHeightRatioPicker(Ratio initialRatio, std::function<void(void)> onUpdate)
	: m_ratioPicker(initialRatio, [this, onUpdate]() {updateWidthOrHeight(); onUpdate(); }),
	  m_onUpdate(onUpdate),
	  m_height(1000), m_lastModified(Height)
{
	onUpdate();
}

void GUIwidget_WidthHeightRatioPicker::Show() {
	m_ratioPicker.Show();
	//
	ImGui::PushItemWidth(100.f);
	ImGui::Text("Width : "); ImGui::SameLine();
	ImGui::PushID(2);
	if (ImGui::InputScalar("", ImGuiDataType_U32, &m_width, NULL, NULL, "%u")) {
		m_lastModified = Width;
		updateWidthOrHeight();
		m_onUpdate();
	}
	ImGui::SameLine();
	ImGui::PopID();
	ImGui::Text("Height : "); ImGui::SameLine();
	ImGui::PushID(3);
	if (ImGui::InputScalar("", ImGuiDataType_U32, &m_height, NULL, NULL, "%u")) {
		m_lastModified = Height;
		updateWidthOrHeight();
		m_onUpdate();
	}
	ImGui::PopID();
	ImGui::PopItemWidth();
}

void GUIwidget_WidthHeightRatioPicker::updateWidthOrHeight() {
	switch (m_lastModified) {
	case Width:
		m_height = ceil(m_width / getRatio());
		break;
	case Height:
		m_width = ceil(m_height * getRatio());
		break;
	default:
		break;
	}
}