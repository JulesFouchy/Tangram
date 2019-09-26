#pragma once

class DrawingBoardGUI {
public:
	static void Show();
	static void OpenCreateShaderLayerWindow();
private:
	static void ImGuiShaderLayerCreationWindow();

private:
	static bool m_bShaderLayerCreationWindow;

private:
	DrawingBoardGUI() = default;
	~DrawingBoardGUI() = default;
};