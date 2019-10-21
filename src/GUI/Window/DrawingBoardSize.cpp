#include "DrawingBoardSize.hpp"

#include "core/drawingBoard.hpp"

#include "imgui/imgui.h"

GUIwindow_DrawingBoardSize::GUIwindow_DrawingBoardSize()
	: GUIwindow("DrawingBoard's AspectRatio"),
	  m_RatioPicker(Ratio(1,1),
		[this]() {
			DrawingBoard::transform.setAspectRatio(m_RatioPicker.getRatio());
		}
	  )
{

}

void GUIwindow_DrawingBoardSize::Open() {
	GUIwindow::Open();
	m_RatioPicker.setRatio(DrawingBoard::transform.getAspectRatio());
}

void GUIwindow_DrawingBoardSize::Show() {
	BeginWindow();
		m_RatioPicker.Show();
	ConfirmationButton();
	EndWindow();
}