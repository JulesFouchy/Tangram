#pragma once
#include "layer.hpp"

class LoadedImageLayer : public Layer {
public:
	LoadedImageLayer(const std::string& imgFilePath);
	~LoadedImageLayer();

	void createACopy() override;

	void drawOnFrameBuffer_Preview(FrameBuffer& frameBuffer) override;
	void drawOnFrameBuffer_Save(FrameBuffer& frameBuffer, int drawingBoardHeight) override;
	inline void computePreviewBuffer() override {};
	inline void computeSaveBuffer(int drawingBoardHeight, RectTransform& transform) override {};

private:
	std::string m_filePath;
};