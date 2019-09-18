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
	void setAspectRatio(float newAspectRatio);

	inline bool isBusy() override { return Transform::isBusy() || bDraggingAspectRatioLead || bDraggingAspectRatioFollow; }

	void startDraggingScale(glm::vec2 dragCenterInDrawingBoardSpace) override;
	void startDraggingAspectRatioLead(AspectRatioDraggingInfo* infos, glm::vec2 originInDrawginBoardSpace);
	void startDraggingAspectRatioFollow(AspectRatioDraggingInfo* infos, glm::vec2 originInDrawginBoardSpace);
	void changeDraggingCenter(glm::vec2 newDraggingCenterInTransformSpace) override;
	void changeDraggingCenterToAltOrigin() override;
	void switchDraggingToRatioFromScale();
	void switchDraggingToScaleFromRatio();
	void checkDragging() override;
	bool endDragging() override;

	void reset() override;
	void pushStateInHistoryAtTheEndOfDragging() override;
private:
	void pushAspectRatioInHistory();
private:
	void scaleU(float scaleFactor, glm::vec2 originInTransformSpace);
	void scaleV(float scaleFactor, glm::vec2 originInTransformSpace);
public:
	MousePositionRelativeToRect getMouseRelativePosition();
private:
	float m_aspectRatio;
	float m_initialAspectRatio;
	glm::mat4x4 m_projectionMatrix;

	bool bMustRecomputeProjMat;

	void startDraggingScaleOrAspectRatio(glm::vec2 dragCenterInDrawingBoardSpace);
	bool bDraggingAspectRatioLead;
	bool bDraggingAspectRatioFollow;
	float m_aspectRatioWhenDraggingStarted;
	AspectRatioDraggingInfo* m_aspectRatioDraggingInfo;
};