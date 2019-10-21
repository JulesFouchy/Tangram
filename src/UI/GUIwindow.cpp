#include "GUIwindow.hpp"

#include "UI/log.hpp"

GUIwindow::GUIwindow()
	: m_bMustShow(false)
{
}

void GUIwindow::Open() {
	m_bMustShow = true;
}

void GUIwindow::Show_IfOpen() {
	if (m_bMustShow) {
		Show();
	}
}