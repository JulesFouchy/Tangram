#include "aspectRatioDraggingInfo.hpp"

#include "UI/input.hpp"
#include "core/drawingBoard.hpp"

AspectRatioDraggingInfo::AspectRatioDraggingInfo(Transform* leadTransform, glm::vec2 originInDrawingBoardSpace, bool unlockU, bool unlockV)
	: m_leadTransform(leadTransform), m_originInDrawingBoardSpace(originInDrawingBoardSpace), m_bUnlockU(unlockU), m_bUnlockV(unlockV),
	m_mousePosWhenDraggingStarted(glm::vec2(0.0f)), m_oneOverInitialMouseRelPosProjOnU(0.0f), m_oneOverInitialMouseRelPosProjOnV(0.0f)
{
	m_originInLeadTransformSpace = leadTransform->getInverseMatrix() * glm::vec4(originInDrawingBoardSpace, 0.0f, 1.0f);
	m_originInWindowSpace = DrawingBoard::transform.getMatrix() * glm::vec4(originInDrawingBoardSpace, 0.0f, 1.0f);
	m_mousePosWhenDraggingStarted = Input::getMousePosition();
	m_oneOverInitialMouseRelPosProjOnU = 1.0f / glm::dot(m_mousePosWhenDraggingStarted - m_originInWindowSpace, getUAxis());
	m_oneOverInitialMouseRelPosProjOnV = 1.0f / glm::dot(m_mousePosWhenDraggingStarted - m_originInWindowSpace, getVAxis());
	int a = 0;
}

AspectRatioDraggingInfo::~AspectRatioDraggingInfo() {

}

glm::vec2 AspectRatioDraggingInfo::getTranslateAmount() {
	return m_leadTransform->getMatrix() * (glm::vec4(m_originInLeadTransformSpace.x, 0.0f, 0.0f, 0.0f) * (1.0f - getUScaleFactor())
						+ glm::vec4(0.0f, m_originInLeadTransformSpace.y, 0.0f, 0.0f) * (1.0f / getVScaleFactor() - 1.0f));
}

float AspectRatioDraggingInfo::getUScaleFactor() {
	if (m_bUnlockU) {
		float du = glm::dot((Input::getMousePosition() - m_originInWindowSpace), getUAxis()) * m_oneOverInitialMouseRelPosProjOnU;
		return du;
	}
	else
		return 1.0f;
}
float AspectRatioDraggingInfo::getVScaleFactor() {
	if (m_bUnlockV) {
		float dv = glm::dot((Input::getMousePosition() - m_originInWindowSpace), getVAxis()) * m_oneOverInitialMouseRelPosProjOnV;
		return dv;
	}
	else
		return 1.0f;
}