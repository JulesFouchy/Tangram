#pragma once

#include "glm/glm.hpp"

#include "transforms/rectTransform.hpp"
#include "graphics/frameBuffer.hpp"

#include "transforms/draggablePoint.hpp"

#include "helper/ratio.hpp"

class Layer {
friend class LayerList;
friend class GroupOfLayers;
friend class DrawingBoard;

public:
	virtual ~Layer();

	void show(const glm::mat4x4& modelMatrix, const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix); // used by copy layer to use it's own modelMatrix
	virtual void show(const glm::mat4x4& viewMatrix, const glm::mat4x4& projMatrix);
	void showSaveTexture();
	void showFrame();
	virtual void showGUI();
	virtual void showDraggablePoints();
	inline virtual DraggablePoint* lookForHoveredDraggablePoint() { return nullptr;	} // nothing by default, until the altOrigin is hooked up to that system

	inline const std::string& getName() { return m_name; }

	virtual inline bool createdSuccessfully() { return true; };
	virtual inline void reload() {}; // nothing for some layerTypes
	inline virtual void createACopy() = 0;
	inline void setTransform(const RectTransform& newTransform) { m_transform = newTransform; }

	inline bool isVisible() { return m_bVisible; }
	inline bool isMovable() { return m_bMovable; }
	inline void setVisibility(bool visible) { m_bVisible = visible; }
	inline void setMovability(bool movable) { m_bMovable = movable; }
	inline bool* getIsVisiblePointer() { return &m_bVisible; } //For ImGui
	inline bool* getIsMovablePointer() { return &m_bMovable; } //For ImGui

	virtual inline FrameBuffer& getFrameBuffer() { return m_previewBuffer; }
	virtual inline Texture2D& getTexture_Preview() { return m_previewBuffer.getTexture(); }
	inline Texture2D& getTexture_Save() { return m_saveBuffer.getTexture(); }

	virtual void drawOnFrameBuffer_Preview(FrameBuffer& frameBuffer) = 0;
	virtual void drawOnFrameBuffer_Save(FrameBuffer& frameBuffer, int drawingBoardHeight) = 0;
	virtual void computePreviewBuffer() = 0;
	virtual void computeSaveBuffer(int drawingBoardHeight, RectTransform& transform) = 0;

protected:
	virtual void onChange();

private:
	bool m_bVisible;
	bool m_bMovable;
protected:
	std::string m_name;
protected:
	FrameBuffer m_previewBuffer;
	FrameBuffer m_saveBuffer;
	bool m_bMustRecomputeSaveBuffer;
public:
	RectTransform m_transform;
protected:
	Layer(Ratio aspectRatio, const std::string& layerName = "New layer"); //Each derived class should initialize aspectRatio and Name in their own way
};