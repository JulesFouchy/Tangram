#include "GUI/Widget/RatioPicker.hpp"

#include "imgui/imgui.h"

#include "core/drawingBoard.hpp"

GUIwidget_RatioPicker::GUIwidget_RatioPicker(Ratio initialRatio, std::function<void(void)> onUpdate)
	: m_ratio(initialRatio), m_onUpdate(onUpdate)
{}

void GUIwidget_RatioPicker::Show() {
	ImGui::PushItemWidth(25.f);
	// Aspect ratio
	ImGui::Text("Ratio : "); ImGui::SameLine();
	ImGui::PushID(0);
	if (ImGui::InputScalar("", ImGuiDataType_U32, m_ratio.getNumeratorPtr(), NULL, NULL, "%u")) {
		m_onUpdate();
	}
	ImGui::SameLine();
	ImGui::PopID();
	ImGui::Text("/"); ImGui::SameLine();
	ImGui::PushID(1);
	if (ImGui::InputScalar("", ImGuiDataType_U32, m_ratio.getDenominatorPtr(), NULL, NULL, "%u")) {
		m_onUpdate();
	}
	ImGui::PopID();
	ImGui::SameLine();
	if (ImGui::Button("Square")) {
		m_ratio.set(1, 1);
		m_onUpdate();
	}
	ImGui::SameLine();
	if (ImGui::Button("Same as DrawingBoard's")) {
		m_ratio = DrawingBoard::transform.getAspectRatio();
		m_onUpdate();
	}
	ImGui::PopItemWidth();
}