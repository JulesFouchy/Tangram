#pragma once

#include "_GUIwindow.hpp"

#include "GUI/Widget/WidthHeightRatioPicker.hpp"
#include "GUI/Widget/FilepathPicker.hpp"

class GUIwindow_ShaderLayerCreation : public GUIwindow {
public:
	GUIwindow_ShaderLayerCreation();
	~GUIwindow_ShaderLayerCreation() = default;

	void Open() override;

private:
	void Show() override;
private:
	GUIwidget_WidthHeightRatioPicker m_WidthHeightRatioPicker;
	GUIwidget_FilepathPicker m_filepathPicker;
};