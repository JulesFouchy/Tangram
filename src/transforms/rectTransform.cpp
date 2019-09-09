#include "rectTransform.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"

#include "UI/input.hpp"

#include "UI/log.hpp"

#include "core/drawingBoard.hpp"

#include "utilities/maths.hpp"

#include <algorithm>

#include "glm/gtc/matrix_inverse.hpp"
RectTransform::RectTransform(float aspectRatio)
	: m_aspectRatio(aspectRatio), m_projectionMatrix(glm::ortho(-0.5f * m_aspectRatio, 0.5f * m_aspectRatio, -0.5f, 0.5f)), bMustRecomputeProjMat(false),
	bDraggingAspectRatio(false), bAspectRatioUUnlocked(false), bAspectRatioVUnlocked(false), m_aspectRatioWhenDraggingStarted(aspectRatio),
	m_oneOverInitialMouseRelPosProjOnU(0.0f), m_oneOverInitialMouseRelPosProjOnV(0.0f)
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

void RectTransform::startDraggingScaleOrAspectRatio(glm::vec2 dragCenterInDrawingBoardSpace) {
	m_matrixWhenDraggingStarted = getMatrix();
	m_scaleWhenDraggingStarted = getScale();
	m_aspectRatioWhenDraggingStarted = getAspectRatio();
	m_translationWhenDraggingStarted = getTranslation();
	m_mousePosWhenDraggingStarted = Input::getMousePosition();

	m_dragCenterInDrawingBoardSpace = dragCenterInDrawingBoardSpace;
	m_dragCenterInTransformSpace = getInverseMatrix() * glm::vec4(m_dragCenterInDrawingBoardSpace, 0.0f, 1.0f);
	m_initialDragCenterInTransformSpace = m_dragCenterInTransformSpace;
	m_dragCenterInWindowSpace = DrawingBoard::transform.getMatrix() * glm::vec4(m_dragCenterInDrawingBoardSpace, 0.0f, 1.0f);
}
void RectTransform::startDraggingScale(glm::vec2 dragCenterInDrawingBoardSpace) {
	bDraggingScale = true;
	startDraggingScaleOrAspectRatio(dragCenterInDrawingBoardSpace);
	computeDraggingScaleVariables(Input::getMousePosition());
}
void RectTransform::computeDraggingRatioVariables() {
	m_oneOverInitialMouseRelPosProjOnU = 1.0f / glm::dot(m_mousePosWhenDraggingStarted - m_dragCenterInWindowSpace, getUAxis());
	m_oneOverInitialMouseRelPosProjOnV = 1.0f / glm::dot(m_mousePosWhenDraggingStarted - m_dragCenterInWindowSpace, getVAxis());
}
void RectTransform::startDraggingAspectRatio(glm::vec2 dragCenterInDrawingBoardSpace) {
	bDraggingAspectRatio = true;
	startDraggingScaleOrAspectRatio(dragCenterInDrawingBoardSpace);
	computeDraggingRatioVariables();
}

void RectTransform::unlockUAspectRatio() {
	bAspectRatioUUnlocked = true;
}

void RectTransform::unlockVAspectRatio() {
	bAspectRatioVUnlocked = true;
}

void RectTransform::changeDraggingRatioOrigin(glm::vec2 newRatioOriginInTransformSpace) {
	if (bDraggingAspectRatio) {
		m_dragCenterInTransformSpace = newRatioOriginInTransformSpace;
		m_dragCenterInWindowSpace = DrawingBoard::transform.getMatrix() * m_matrixWhenDraggingStarted * glm::vec4(newRatioOriginInTransformSpace, 0.0f, 1.0f);
		computeDraggingRatioVariables();
	}
}
void RectTransform::revertDraggingRatioToInitialOrigin() {
	changeDraggingRatioOrigin(m_initialDragCenterInTransformSpace);
}
void RectTransform::changeDraggingRatioToAltOrigin() {
	changeDraggingRatioOrigin(getAltOriginInTransformSpace() * glm::vec2(m_aspectRatioWhenDraggingStarted / getAspectRatio() ,1.0f));
}
void RectTransform::switchDraggingToRatioFromScale() {
	if (bDraggingScale) {
		bDraggingScale = false;
		bDraggingAspectRatio = true;

		computeDraggingRatioVariables();

		checkDragging();
	}
}
void RectTransform::switchDraggingToScaleFromRatio(){
	if (bDraggingAspectRatio) {
		bDraggingScale = true;
		bDraggingAspectRatio = false;
		setAspectRatio(m_aspectRatioWhenDraggingStarted);

		computeDraggingScaleVariables(m_mousePosWhenDraggingStarted);

		checkDragging();
	}
}

void RectTransform::checkDragging() {
	Transform::checkDragging();
	if (bDraggingAspectRatio) {
		float newAspectRatio = m_aspectRatioWhenDraggingStarted;
		float newScale = m_scaleWhenDraggingStarted;
		glm::vec2 newTranslation = glm::vec2(0.0f);
		if (bAspectRatioUUnlocked) {
			float du = glm::dot((Input::getMousePosition() - m_dragCenterInWindowSpace), getUAxis()) * m_oneOverInitialMouseRelPosProjOnU;
			newAspectRatio *= du;
			newTranslation.x = m_dragCenterInTransformSpace.x * (1.0f - du);
		}
		if (bAspectRatioVUnlocked) {
			float dv = glm::dot((Input::getMousePosition() - m_dragCenterInWindowSpace), getVAxis()) * m_oneOverInitialMouseRelPosProjOnV;
			newAspectRatio /= dv;
			newScale *= dv;
			newTranslation.x /= dv;
			newTranslation.y = m_dragCenterInTransformSpace.y * (1.0f / dv - 1.0f);
		}
		setScale(newScale);
		setAspectRatio(newAspectRatio);
		setTranslation(glm::vec4(m_translationWhenDraggingStarted, 0.0f, 0.0f) + getMatrix() * glm::vec4(newTranslation, 0.0f, 0.0f));
	}
}

bool RectTransform::endDragging() {
	bool handled = Transform::endDragging() || bDraggingAspectRatio;
	bDraggingAspectRatio = false;
	bAspectRatioUUnlocked = false;
	bAspectRatioVUnlocked = false;
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