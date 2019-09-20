#pragma once

#include "layer.hpp"

class CopyLayer : public Layer {
public:
	CopyLayer(Layer* childLayer);
	~CopyLayer();

	void show(const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix) override;
	void showInDrawingBoardSpace() override;

	inline Texture2D& getTexture() override { return m_childLayer->getTexture(); }
private:
	Layer* m_childLayer;
};