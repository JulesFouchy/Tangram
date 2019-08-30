#pragma once

#include "glm/glm.hpp"

#include "graphics/loadedImage.hpp"
#include "transforms/rectTransform.hpp"

class Layer {
friend class LayerList;
friend class GroupOfLayers;
friend class DrawingBoard;

public:
	Layer(std::string imgFilePath, std::string layerName = "New layer");
	~Layer();

	void show(glm::mat4x4 viewMatrix, glm::mat4x4 projMatrix);
	void showInDrawingBoardSpace();
	void showFrame();

	inline const std::string& getName() { return m_name; }

	inline bool isVisible() { return m_bVisible; }
	inline void setVisibility(bool visible) { m_bVisible = visible; }
	inline bool* getIsVisiblePointer() { return &m_bVisible; }

	inline Texture2D& getTexture() { return m_image; }

private:
	bool m_bVisible;
private:
	LoadedImage m_image;
	std::string m_name;
public:
	RectTransform m_transform;
};