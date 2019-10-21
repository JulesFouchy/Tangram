#pragma once

#include "_GUIwindow.hpp"

#include "GUI/Widget/RatioPicker.hpp"

class GUIwindow_DrawingBoardSize : public GUIwindow {
public:
	GUIwindow_DrawingBoardSize();
	~GUIwindow_DrawingBoardSize() = default;

private:
	void Show() override;
private:
	GUIwidget_RatioPicker m_RatioPicker;
};