#pragma once

#include "_GUIwindow.hpp"

#include "GUI/Widget/WidthHeightRatioPicker.hpp"
#include "GUI/Widget/FilepathPicker.hpp"

#include "UI/log.hpp"

#include "core/layerID.hpp"

class GUIwindow_EffectLayerCreation : public GUIwindow {
public:
	GUIwindow_EffectLayerCreation();
	~GUIwindow_EffectLayerCreation() = default;

	inline void Open() override { spdlog::error("[GUIwindow_EffectLayerCreation::Open] must give a targetLayerID as a parameter"); }
	void Open(LayerID targetLayerID);

private:
	void Show() override;
private:
	LayerID m_targetLayerID;
	GUIwidget_WidthHeightRatioPicker m_WidthHeightRatioPicker;
	GUIwidget_FilepathPicker m_filepathPicker;
};