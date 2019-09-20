#pragma once

#include "layer.hpp"

class CopyLayer : public Layer {
public:
	CopyLayer(Layer* childLayer);
	~CopyLayer();
private:
	Layer* m_childLayer;
};