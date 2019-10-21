#pragma once

#include <functional>

class GUIwindow {
public:
	GUIwindow(const std::string& windowName, std::function<void(void)> OnConfirmation = []() {});
	~GUIwindow() = default;

	virtual void Open();
	void Show_IfOpen();
protected:
	virtual void Show() = 0;
	void BeginWindow();
	void ConfirmationButton();
	void EndWindow();
protected:
	bool m_bMustShow;
private:
	std::string m_name;
	std::function<void(void)> m_OnConfirmation;
};