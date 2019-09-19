#pragma once

#include "transform.hpp"

#include "aspectRatioDraggingInfo.hpp"

class GroupTransform : public Transform {
public:
	GroupTransform();
	~GroupTransform();

	void startDraggingScale(Transform* leadTransform, glm::vec2 originInDrawingBoardSpace, bool unlockU, bool unlockV);

	inline AspectRatioDraggingInfo* getAspectRatioDraggingInfos() { return &m_draggingRatioInfos; }
private:
	AspectRatioDraggingInfo m_draggingRatioInfos;
};