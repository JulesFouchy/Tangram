#pragma once

#include "glm/glm.hpp"

#include "transforms/rectTransform.hpp"
#include "graphics/frameBuffer.hpp"

#include "helper/ratio.hpp"

class Layer {
friend class LayerList;
friend class GroupOfLayers;
friend class DrawingBoard;

public:
	virtual ~Layer();

	void show(const glm::mat4x4& modelMatrix, const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix); // used by copy layer to use it's own modelMatrix
	virtual void show(const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix);
	void showForSaving();
	virtual void showForSaving(RectTransform& transform) = 0; // allows copy layer to draw it with its own transform
	void showFrame();
	virtual void showGUI();
	virtual void showDraggablePoints();

	inline const std::string& getName() { return m_name; }

	virtual inline bool createdSuccessfully() { return true; };
	virtual inline void reload() {}; // nothing for some layerTypes

	inline bool isVisible() { return m_bVisible; }
	inline bool isMovable() { return m_bMovable; }
	inline void setVisibility(bool visible) { m_bVisible = visible; }
	inline void setMovability(bool movable) { m_bMovable = movable; }
	inline bool* getIsVisiblePointer() { return &m_bVisible; } //For ImGui
	inline bool* getIsMovablePointer() { return &m_bMovable; } //For ImGui

	virtual inline Texture2D& getTexture() { return m_renderBuffer.getTexture(); }
	virtual inline FrameBuffer& getFrameBuffer() { return m_renderBuffer; }

private:
	bool m_bVisible;
	bool m_bMovable;
protected:
	std::string m_name;
protected:
	FrameBuffer m_renderBuffer;
public:
	RectTransform m_transform;
protected:
	Layer(Ratio aspectRatio, const std::string& layerName = "New layer"); //Each derived class should initialize aspectRatio and Name in their own way
};