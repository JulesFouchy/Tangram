#pragma once

#include "helper/ratio.hpp"
#include <functional>

class GUIwidget_RatioPicker {
friend class GUIwidget_WidthHeightRatioPicker;
public:
	GUIwidget_RatioPicker(Ratio initialRatio, std::function<void(void)> onUpdate = []() {});
	~GUIwidget_RatioPicker() = default;

	void Show();
	inline Ratio getRatio() const { return m_ratio; }
	inline void setRatio(const Ratio& ratio) { m_ratio = ratio; onUpdate(); }

private:
	void onUpdate();
private:
	Ratio m_ratio;
	std::function<void(void)> m_onUpdate;
};