#pragma once

#include "_GUIwindow.hpp"

#include "GUI/Widget/WidthHeightRatioPicker.hpp"
#include "GUI/Widget/FilepathPicker.hpp"

class GUIwindow_ExportImage : public GUIwindow {
public:
	GUIwindow_ExportImage();
	~GUIwindow_ExportImage() = default;

	void Open() override;

private:
	void Show() override;
private:
	GUIwidget_WidthHeightRatioPicker m_WidthHeightRatioPicker;
	GUIwidget_FilepathPicker m_FilepathPicker;
};