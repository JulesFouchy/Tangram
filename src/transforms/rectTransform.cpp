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
	: m_aspectRatio(aspectRatio), m_initialAspectRatio(aspectRatio), m_projectionMatrix(glm::ortho(-0.5f * m_aspectRatio, 0.5f * m_aspectRatio, -0.5f, 0.5f)),
	bMustRecomputeProjMat(false),
	m_aspectRatioWhenDraggingStarted(aspectRatio), m_aspectRatioDraggingInfo(),
	bDraggingAspectRatio(false), m_dragRatioIsFollowingAnotherLayer(false)
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

void RectTransform::setAspectRatio(float newAspectRatio, bool bPushChangeInHistory) {
	setAltOrigin(glm::vec2(getAltOriginInTransformSpace().x / m_aspectRatio * newAspectRatio, getAltOriginInTransformSpace().y), bPushChangeInHistory);
	if (bPushChangeInHistory) {
		// Get initial value
		float initialRatio = getAspectRatio();
		//
		if (initialRatio != newAspectRatio) {
			// Push state
			DrawingBoard::history.addAction(Action(
				// DO action
				[this, newAspectRatio]()
			{
				setAspectRatio(newAspectRatio);
			},
				// UNDO action
				[this, initialRatio]()
			{
				setAspectRatio(initialRatio);
			}
			));
		}
	}
	m_aspectRatio = newAspectRatio;
	bMustRecomputeProjMat = true;
}

void RectTransform::reset(bool bPushChangeInHistory) {
	Transform::reset(bPushChangeInHistory);
	setAspectRatio(m_initialAspectRatio, bPushChangeInHistory);
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
void RectTransform::startDraggingAspectRatio(AspectRatioDraggingInfo* infos, glm::vec2 originInDrawginBoardSpace, bool bFollowingAnotherLayer) {
	bDraggingAspectRatio = true;
	m_dragRatioIsFollowingAnotherLayer = bFollowingAnotherLayer;
	m_aspectRatioDraggingInfo = infos;
	startDraggingScaleOrAspectRatio(originInDrawginBoardSpace);
}

void RectTransform::changeDraggingCenter(glm::vec2 newDraggingCenterInTransformSpace) {
	Transform::changeDraggingCenter(newDraggingCenterInTransformSpace);
}
void RectTransform::changeDraggingCenterToAltOrigin() {
	changeDraggingCenter(getAltOriginInTransformSpace() * glm::vec2(m_aspectRatioWhenDraggingStarted / getAspectRatio() ,1.0f));
}
void RectTransform::switchDraggingToRatioFromScale() {
	if (bDraggingScale) {
		bDraggingScale = false;
		//bDraggingAspectRatio = true;

		//computeDraggingRatioVariables();

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
		if (!m_dragRatioIsFollowingAnotherLayer) {

			float scaleFactorU = m_aspectRatioDraggingInfo->getUScaleFactor();
			float scaleFactorV = m_aspectRatioDraggingInfo->getVScaleFactor();

			setAspectRatio(m_aspectRatioWhenDraggingStarted);
			setScale(m_scaleWhenDraggingStarted);
			setTranslation(m_translationWhenDraggingStarted);

			scaleU(scaleFactorU, m_dragCenterInTransformSpace);
			scaleV(scaleFactorV, m_dragCenterInTransformSpace);
		}

		else {
			setAspectRatio(m_aspectRatioWhenDraggingStarted);
			setScale(m_scaleWhenDraggingStarted);

			float uScale = m_aspectRatioDraggingInfo->getUScaleFactor();
			float vScale = m_aspectRatioDraggingInfo->getVScaleFactor();
			glm::vec2 dl = m_translationWhenDraggingStarted - m_aspectRatioDraggingInfo->getTranslationWhenDraggingStarted();
			setTranslation(m_translationWhenDraggingStarted
				+ m_aspectRatioDraggingInfo->getTranslateAmount()
				+ glm::dot(dl, m_aspectRatioDraggingInfo->getUAxis()) * uScale * m_aspectRatioDraggingInfo->getUAxis()
				+ glm::dot(dl, m_aspectRatioDraggingInfo->getVAxis()) * vScale * m_aspectRatioDraggingInfo->getVAxis()
				- dl
			);
			float t = Maths::modulo(getRotation() - m_aspectRatioDraggingInfo->getRotation(), Maths::PI) / Maths::PI;
			if (t < 0.5f)
				t *= 2.0f;
			else
				t = 2.0f * (1.0f - t);
			scaleU(pow(abs(uScale), 1.0f - t) * pow(abs(vScale), t), glm::vec2(0.0f));
			scaleV(pow(abs(uScale), t) * pow(abs(vScale), 1.0f - t), glm::vec2(0.0f));
		}
	}
}

bool RectTransform::endDragging() {
	bool handled = Transform::endDragging() || bDraggingAspectRatio;
	bDraggingAspectRatio = false;
	return handled;
}

void RectTransform::pushStateInHistory() {
	// Don't forget to call this function between history.beginUndoGroup() and history.endUndoGroup() !!!
	Transform::pushStateInHistory();
	pushAspectRatioInHistoryAtTheEndOfDragging();
}

void RectTransform::pushStateInHistoryAtTheEndOfDragging() {
	// N.B. : begin/end undoGroup are called by GroupOfLayers
	Transform::pushStateInHistoryAtTheEndOfDragging();
	if (bDraggingAspectRatio) {
		pushAspectRatioInHistoryAtTheEndOfDragging();
		pushScaleInHistoryAtTheEndOfDragging();
		pushTranslationInHistoryAtTheEndOfDragging();
	}
}

void RectTransform::pushAspectRatioInHistoryAtTheEndOfDragging() {
	// Get values
	float ratio = getAspectRatio();
	float initialRatio = m_aspectRatioWhenDraggingStarted;
	if (ratio != initialRatio) {
		// Push state
		DrawingBoard::history.addAction(Action(
			// DO action
			[this, ratio]()
		{
			setAspectRatio(ratio);
		},
			// UNDO action
			[this, initialRatio]()
		{
			setAspectRatio(initialRatio);
		}
		));
	}
}

void RectTransform::scaleU(float scaleFactor, glm::vec2 originInTransformSpace, bool bPushChangeInHistory) {
	setAspectRatio(getAspectRatio() * scaleFactor, bPushChangeInHistory);
	translate(getMatrix() * glm::vec4(originInTransformSpace.x, 0.0f, 0.0f, 0.0f) * (1.0f - scaleFactor), bPushChangeInHistory);
}
void RectTransform::scaleV(float scaleFactor, glm::vec2 originInTransformSpace, bool bPushChangeInHistory) {
	setAspectRatio(getAspectRatio() / scaleFactor, bPushChangeInHistory);
	setScale(getScale() * scaleFactor, bPushChangeInHistory);
	translate(getMatrix() * glm::vec4(0.0f, originInTransformSpace.y, 0.0f, 0.0f) * (1.0f / scaleFactor - 1.0f), bPushChangeInHistory);
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