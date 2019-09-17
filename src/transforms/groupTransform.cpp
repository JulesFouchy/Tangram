#include "groupTransform.hpp"

#include "UI/input.hpp"

GroupTransform::GroupTransform()
{

}
GroupTransform::~GroupTransform() {

}

void GroupTransform::startDraggingAspectRatio(Transform* leadTransform, glm::vec2 originInDrawingBoardSpace, bool unlockU, bool unlockV) {
	m_draggingRatioInfos = AspectRatioDraggingInfo(leadTransform, originInDrawingBoardSpace, unlockU, unlockV);
}