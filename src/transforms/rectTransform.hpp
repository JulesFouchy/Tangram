#pragma once

#include "transform.hpp"
#include "groupTransform.hpp"
#include "aspectRatioDraggingInfo.hpp"

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
	void setAspectRatio(float newAspectRatio, bool bPushChangeInHistory = false);

	inline bool isBusy() override { return Transform::isBusy() || bDraggingAspectRatio; }

	void startDraggingScale(glm::vec2 dragCenterInDrawingBoardSpace) override;
	void startDraggingAspectRatio(AspectRatioDraggingInfo* infos, glm::vec2 originInDrawginBoardSpace, bool bFollowingAnotherLayer);
	void changeDraggingCenter(glm::vec2 newDraggingCenterInTransformSpace) override;
	void changeDraggingCenterToAltOrigin() override;
	void switchDraggingToRatioFromScale();
	void switchDraggingToScaleFromRatio();
	void checkDragging() override;
	bool endDragging() override;

	void reset(bool bPushChangeInHistory = false) override;
	void pushStateInHistory() override;
	void pushStateInHistoryAtTheEndOfDragging() override;
private:
	void pushAspectRatioInHistoryAtTheEndOfDragging();
private:
	void scaleU(float scaleFactor, glm::vec2 originInTransformSpace, bool bPushChangeInHistory = false);
	void scaleV(float scaleFactor, glm::vec2 originInTransformSpace, bool bPushChangeInHistory = false);
public:
	MousePositionRelativeToRect getMouseRelativePosition();
private:
	float m_aspectRatio;
	float m_initialAspectRatio;
	glm::mat4x4 m_projectionMatrix;

	bool bMustRecomputeProjMat;

	void startDraggingScaleOrAspectRatio(glm::vec2 dragCenterInDrawingBoardSpace);
	bool bDraggingAspectRatio;
	bool m_dragRatioIsFollowingAnotherLayer;
	float m_aspectRatioWhenDraggingStarted;
	AspectRatioDraggingInfo* m_aspectRatioDraggingInfo;
};