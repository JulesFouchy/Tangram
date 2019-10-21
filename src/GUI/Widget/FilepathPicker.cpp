#include "FilepathPicker.hpp"

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "UI/fileBrowser.hpp"

GUIwidget_FilepathPicker::GUIwidget_FilepathPicker(const char* fileFilter)
	: m_filepath(""), m_fileFilter(fileFilter)
{

}

void GUIwidget_FilepathPicker::Show() {
	ImGui::InputText("", &m_filepath);
	if (ImGui::Button("Choose file")) {
		std::string tmp = FileBrowser::openfilename(m_fileFilter);
		if (tmp != "") {
			m_filepath = tmp;
		}
	}
}