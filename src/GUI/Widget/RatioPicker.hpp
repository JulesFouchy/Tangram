#pragma once

#include "helper/ratio.hpp"
#include <functional>

enum ButtonFlag {
	ButtonFlag_Square             = 1 << 0,
	ButtonFlag_SameAsDrawingBoard = 1 << 1
};

class GUIwidget_RatioPicker {
friend class GUIwidget_WidthHeightRatioPicker;
public:
	GUIwidget_RatioPicker(int buttonFlags = ButtonFlag_Square & ButtonFlag_SameAsDrawingBoard, Ratio initialRatio = Ratio(1,1), std::function<void(void)> onUpdate = []() {});
	~GUIwidget_RatioPicker() = default;

	void Show();
	inline const Ratio& getRatio() const { return m_ratio; }
	inline void setRatio(const Ratio& ratio) { m_ratio = ratio; onUpdate(); }

private:
	void onUpdate();
private:
	Ratio m_ratio;
	int m_buttonFlags;
	std::function<void(void)> m_onUpdate;
};