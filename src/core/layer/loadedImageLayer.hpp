#pragma once
#include "layer.hpp"

class LoadedImageLayer : public Layer {
public:
	LoadedImageLayer(const std::string& imgFilePath);
	~LoadedImageLayer();

	void showForSaving(Transform& transform) override;
private:
	std::string m_filePath;
	unsigned char* m_pixels;
};