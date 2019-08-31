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

	inline float getAspectRatio() { return m_aspectRatio; };
	void setAspectRatio(float newAspectRatio);

	const glm::mat4x4& getProjectionMatrix();

	inline bool isBusy() override { return Transform::isBusy() || bDraggingAspectRatio; }

	void startDraggingAspectRatio();
	void checkDragging() override;
	bool endDragging() override;

	MousePositionRelativeToRect getMouseRelativePosition();

private:
	float m_aspectRatio;
	glm::mat4x4 m_projectionMatrix;

	bool bMustRecomputeProjMat;

	bool bDraggingAspectRatio;
	float m_aspectRatioWhenDraggingStarted;
};