#include "rectTransform.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "UI/input.hpp"

#include "UI/log.hpp"

#include "core/drawingBoard.hpp"

#include "utilities/maths.hpp"

#include <algorithm>

#include "glm/gtc/matrix_inverse.hpp"
RectTransform::RectTransform(float aspectRatio)
	: m_aspectRatio(aspectRatio), m_projectionMatrix(glm::ortho(-0.5f * m_aspectRatio, 0.5f * m_aspectRatio, -0.5f, 0.5f)), bMustRecomputeProjMat(false),
	bDraggingAspectRatioH(false), bDraggingAspectRatioV(false), m_aspectRatioWhenDraggingStarted(aspectRatio), m_dragCenterInTransformSpace(glm::vec2(0.0f))
{
}
RectTransform::~RectTransform(){
}

const glm::mat4x4& RectTransform::getProjectionMatrix() {
	if (bMustRecomputeProjMat) {
		m_projectionMatrix = glm::ortho(-0.5f * m_aspectRatio, 0.5f * m_aspectRatio, -0.5f, 0.5f);
		bMustRecomputeProjMat = false;
	}
	return m_projectionMatrix;
}

void RectTransform::setAspectRatio(float newAspectRatio) {
	setAltOrigin(glm::vec2(getAltOriginInTransformSpace().x / m_aspectRatio * newAspectRatio, getAltOriginInTransformSpace().y));
	m_aspectRatio = newAspectRatio;
	bMustRecomputeProjMat = true;
}

void RectTransform::startDraggingAspectRatioH(glm::vec2 dragCenterInTransformSpace) {
	bDraggingAspectRatioH = true;
	m_scaleWhenDraggingStarted = getScale();

	m_translationWhenDraggingStarted = getTranslation();
	m_aspectRatioWhenDraggingStarted = m_aspectRatio;
	m_mousePosWhenDraggingStarted = Input::getMousePosition();
	m_dragCenterInTransformSpace = dragCenterInTransformSpace;
	m_dragCenterInWindowSpace = DrawingBoard::transform.getMatrix() * getMatrix() * glm::vec4(dragCenterInTransformSpace, 0.0f, 1.0f);
}

void RectTransform::startDraggingAspectRatioV(glm::vec2 dragCenterInTransformSpace) {
	bDraggingAspectRatioV = true;
	m_scaleWhenDraggingStarted = getScale();

	m_translationWhenDraggingStarted = getTranslation();
	m_aspectRatioWhenDraggingStarted = m_aspectRatio;
	m_mousePosWhenDraggingStarted = Input::getMousePosition();
	m_dragCenterInTransformSpace = dragCenterInTransformSpace;
	m_dragCenterInWindowSpace = DrawingBoard::transform.getMatrix() * getMatrix() * glm::vec4(dragCenterInTransformSpace, 0.0f, 1.0f);
}


void RectTransform::checkDragging() {
	Transform::checkDragging();
	float newAspectRatio = m_aspectRatioWhenDraggingStarted;
	float newScale = m_scaleWhenDraggingStarted;
	glm::vec2 newTranslation = glm::vec2(0.0f);
	if (bDraggingAspectRatioH) {
		float du = glm::dot((Input::getMousePosition() - m_dragCenterInWindowSpace), getUAxis()) / glm::dot( m_mousePosWhenDraggingStarted - m_dragCenterInWindowSpace, getUAxis());
		newAspectRatio *= du;
		newTranslation.x = m_dragCenterInTransformSpace.x * (1.0f - du);
	}
	if (bDraggingAspectRatioV) {
		float dv = glm::dot((Input::getMousePosition() - m_dragCenterInWindowSpace), getVAxis()) / glm::dot(m_mousePosWhenDraggingStarted - m_dragCenterInWindowSpace, getVAxis());
		newAspectRatio /= dv;
		newScale *= dv;
		newTranslation.x /= dv;
		newTranslation.y = m_dragCenterInTransformSpace.y * (1.0f/dv - 1.0f);
	}
	if (bDraggingAspectRatioH || bDraggingAspectRatioV) {
		setScale(newScale);
		setAspectRatio(newAspectRatio);
		setTranslation(glm::vec4(m_translationWhenDraggingStarted, 0.0f, 0.0f) + getMatrix() * glm::vec4(newTranslation, 0.0f, 0.0f));
	}
}

bool RectTransform::endDragging() {
	bool handled = Transform::endDragging() || bDraggingAspectRatioH || bDraggingAspectRatioV;
	bDraggingAspectRatioH = false;
	bDraggingAspectRatioV = false;
	return handled;
}

MousePositionRelativeToRect RectTransform::getMouseRelativePosition() {
	glm::vec4 dl = glm::inverse(DrawingBoard::transform.getMatrix() * glm::scale(getMatrix(), glm::vec3(getAspectRatio(), 1.0f, 1.0f))) * glm::vec4(Input::getMousePosition(), 0.0f, 1.0f);
	float margin = 0.025f;
	int xPos, yPos;
	//------Check y------
	if (dl.y < -0.5f - margin)
		return OUTSIDE;
	else if (dl.y < -0.5f + margin)
		yPos = 0; //a.k.a BOT
	else if (dl.y < 0.5f - margin)
		yPos = 1; //a.k.a. INSIDE
	else if (dl.y < 0.5f + margin)
		yPos = 2; //a.k.a. TOP
	else
		return OUTSIDE;
	//------Check x------
	if (dl.x < -0.5f - margin)
		return OUTSIDE;
	else if (dl.x < -0.5f + margin)
		xPos = 0; //a.k.a LEFT
	else if (dl.x < 0.5f - margin)
		xPos = 1; //a.k.a. INSIDE
	else if (dl.x < 0.5f + margin)
		xPos = 2; //a.k.a. RIGHT
	else
		return OUTSIDE;
	//------Mix x and y------
	switch (yPos + 3*xPos) {
	case 0:
		return BOT_LEFT;
	break;
	case 1:
		return LEFT;
	break;
	case 2:
		return TOP_LEFT;
	break;
	case 3:
		return BOT;
	break;
	case 4:
		return INSIDE;
	break;
	case 5:
		return TOP;
	break;
	case 6:
		return BOT_RIGHT;
	break;
	case 7:
		return RIGHT;
	break;
	case 8:
		return TOP_RIGHT;
	break;
	}
}