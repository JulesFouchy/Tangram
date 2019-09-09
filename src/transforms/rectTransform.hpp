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

	inline bool isBusy() override { return Transform::isBusy() || bDraggingAspectRatio; }

	void startDraggingScale(glm::vec2 dragCenterInDrawingBoardSpace) override;
	void startDraggingAspectRatio(glm::vec2 dragCenterInDrawingBoardSpace);
	void unlockUAspectRatio();
	void unlockVAspectRatio();
	void changeDraggingRatioOrigin(glm::vec2 newRatioOriginInTransformSpace);
	void revertDraggingRatioToInitialOrigin();
	void changeDraggingRatioToAltOrigin();
	void switchDraggingToRatioFromScale();
	void switchDraggingToScaleFromRatio();
	void checkDragging() override;
	bool endDragging() override;

	MousePositionRelativeToRect getMouseRelativePosition();

private:
	float m_aspectRatio;
	glm::mat4x4 m_projectionMatrix;

	bool bMustRecomputeProjMat;

	void startDraggingScaleOrAspectRatio(glm::vec2 dragCenterInDrawingBoardSpace);
	void computeDraggingRatioVariables();
	bool bDraggingAspectRatio;
	bool bAspectRatioUUnlocked;
	bool bAspectRatioVUnlocked;
	float m_aspectRatioWhenDraggingStarted;
	float m_oneOverInitialMouseRelPosProjOnU;
	float m_oneOverInitialMouseRelPosProjOnV;
};