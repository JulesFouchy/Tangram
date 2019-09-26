#pragma once

enum WidthOrHeight {
	Width,
	Height
};

class GUI_LayerCreation {
public:
	static void Show();
	static void OpenCreateShaderLayerWindow();
private:
	static void Window_ShaderLayerCreation();

	static void ImGuiChoose_Ratio_Width_Height();

private:
	static bool m_bWindow_ShaderLayerCreation;

	static unsigned int m_aspectRatioNumerator;
	static unsigned int m_aspectRatioDenominator;
	static unsigned int m_width;
	static unsigned int m_height;
	static WidthOrHeight m_lastModified;
	static void updateWidthOrHeight();

	static float getAspectRatio() { return (float) m_aspectRatioNumerator / m_aspectRatioDenominator; }

private:
	GUI_LayerCreation() = default;
	~GUI_LayerCreation() = default;
};