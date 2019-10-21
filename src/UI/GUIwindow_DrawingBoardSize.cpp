#include "GUIwindow_DrawingBoardSize.hpp"

#include "core/drawingBoard.hpp"

#include "imgui/imgui.h"

GUIwindow_DrawingBoardSize::GUIwindow_DrawingBoardSize()
	: m_RatioPicker(DrawingBoard::transform.getAspectRatio(),
		[this]() {
			DrawingBoard::transform.setAspectRatio(m_RatioPicker.getRatio());
		}
	  )
{

}

void GUIwindow_DrawingBoardSize::Show() {
	ImGui::Begin("DrawingBoard's AspectRatio", &m_bMustShow);
	m_RatioPicker.Show();
	// creation	
	if (ImGui::Button("OK !")) {
		m_bMustShow = false;
	}
	ImGui::End();
}