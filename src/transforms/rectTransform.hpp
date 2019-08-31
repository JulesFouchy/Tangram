#pragma once

#include "transform.hpp"

enum MousePositionRelativeToRect {
	OUTSIDE,
	INSIDE,
	RIGHT,
	TOP_RIGHT,
	TOP,
	TOP_LEFT,
	LEFT,
	BOT_LEFT,
	BOT,
	BOT_RIGHT
};

class RectTransform : public Transform {
public:
	RectTransform(float aspectRatio);
	virtual ~RectTransform();

	const glm::mat4x4& getProjectionMatrix();

	inline float getAspectRatio() { return m_aspectRatio; };
	void setAspectRatio(float newAspectRatio);

	inline bool isBusy() override { return Transform::isBusy() || bDraggingAspectRatioH || bDraggingAspectRatioV; }

	void startDraggingAspectRatioH(glm::vec2 dragCenterInTransformSpace);
	void startDraggingAspectRatioV(glm::vec2 dragCenterInTransformSpace);
	void checkDragging() override;
	bool endDragging() override;

	MousePositionRelativeToRect getMouseRelativePosition();

private:
	float m_aspectRatio;
	glm::mat4x4 m_projectionMatrix;

	bool bMustRecomputeProjMat;

	bool bDraggingAspectRatioH;
	bool bDraggingAspectRatioV;
	float m_aspectRatioWhenDraggingStarted;
	glm::vec2 m_dragCenterInTransformSpace;
};