#pragma once

#include "glm/glm.hpp"

#include "transforms/rectTransform.hpp"
#include "graphics/texture2D.hpp"

class Layer {
friend class LayerList;
friend class GroupOfLayers;
friend class DrawingBoard;

public:
	~Layer();

	void show(glm::mat4x4 viewMatrix, glm::mat4x4 projMatrix);
	void showInDrawingBoardSpace(); // used when saving
	void showFrame();

	inline const std::string& getName() { return m_name; }

	inline bool isVisible() { return m_bVisible; }
	inline void setVisibility(bool visible) { m_bVisible = visible; }
	inline bool* getIsVisiblePointer() { return &m_bVisible; } //For ImGui

	inline Texture2D& getTexture() { return m_displayTexture; }

private:
	bool m_bVisible;
protected:
	std::string m_name;
protected:
	Texture2D m_displayTexture;
public:
	RectTransform m_transform;
protected:
	Layer(float aspectRatio, const std::string& layerName = "New layer");
};