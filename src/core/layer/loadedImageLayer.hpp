#pragma once
#include "layer.hpp"

class LoadedImageLayer : public Layer {
public:
	LoadedImageLayer(const std::string& imgFilePath);
	~LoadedImageLayer();

	void showForSaving(RectTransform& transform) override;
private:
	std::string m_filePath;
};