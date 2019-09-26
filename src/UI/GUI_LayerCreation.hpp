#pragma once

class GUI_LayerCreation {
public:
	static void Show();
	static void OpenCreateShaderLayerWindow();
private:
	static void ImGuiShaderLayerCreationWindow();

private:
	static bool m_bShaderLayerCreationWindow;

private:
	GUI_LayerCreation() = default;
	~GUI_LayerCreation() = default;
};