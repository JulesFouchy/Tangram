#include "_GUIwindow.hpp"

#include "UI/log.hpp"

#include "imgui/imgui.h"

GUIwindow::GUIwindow(const std::string& windowName, std::function<void(void)> OnConfirmation)
	: m_bMustShow(false), m_name(windowName), m_OnConfirmation(OnConfirmation)
{
}

void GUIwindow::Open() {
	m_bMustShow = true;
}

void GUIwindow::Show_IfOpen() {
	if (m_bMustShow) {
		Show();
	}
}

void GUIwindow::BeginWindow() {
	ImGui::Begin(m_name.c_str(), &m_bMustShow);
}

void GUIwindow::ConfirmationButton() {
	if (ImGui::Button("OK !")) {
		m_bMustShow = false;
		m_OnConfirmation();
	}
}

void GUIwindow::EndWindow() {
	ImGui::End();
}