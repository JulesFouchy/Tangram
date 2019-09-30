#pragma once

#include "helper/ratio.hpp"

#include <string>

enum WidthOrHeight {
	Width,
	Height
};

class GUI_LayerCreation {
public:
	static void Show();
	static void OpenCreateShaderLayerWindow();
	static void OpenSaveDrawingBoardWindow();
private:
	static void Window_ShaderLayerCreation();
	static void Window_DrawingBoardSaving();

	static void ImGuiChoose_Width_Height();
	static void ImGuiChoose_Ratio_Width_Height();
	static void ImGuiOpenFileButton(const char* filter = "All Files (*.*)\0*.*\0");
	static void ImGuiSaveFileNameButton(const char* filter = "All Files (*.*)\0*.*\0");

private:
	static bool m_bWindow_ShaderLayerCreation;
	static bool m_bWindow_DrawingBoardSaving;
	inline static bool isBusy() { return m_bWindow_ShaderLayerCreation || m_bWindow_DrawingBoardSaving; }

	static Ratio m_aspectRatio;
	static unsigned int m_width;
	static unsigned int m_height;
	static WidthOrHeight m_lastModified;
	static void updateWidthOrHeight();

	static std::string m_filepath;
	static int m_fileExtensionIndex;

private:
	GUI_LayerCreation() = default;
	~GUI_LayerCreation() = default;
};