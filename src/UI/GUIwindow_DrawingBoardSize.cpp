#include "GUIwindow_DrawingBoardSize.hpp"

#include "core/drawingBoard.hpp"

#include "imgui/imgui.h"

GUIwindow_DrawingBoardSize::GUIwindow_DrawingBoardSize()
	: GUIwindow("DrawingBoard's AspectRatio"),
	  m_RatioPicker(DrawingBoard::transform.getAspectRatio(),
		[this]() {
			DrawingBoard::transform.setAspectRatio(m_RatioPicker.getRatio());
		}
	  )
{

}

void GUIwindow_DrawingBoardSize::Show() {
	BeginWindow();
		m_RatioPicker.Show();
	ConfirmationButton();
	EndWindow();
}