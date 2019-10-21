#pragma once

class GUIwindow {
public:
	GUIwindow();
	~GUIwindow() = default;

	void Open();
	void Show_IfOpen();
protected:
	virtual void Show() = 0;

protected:
	bool m_bMustShow;
};