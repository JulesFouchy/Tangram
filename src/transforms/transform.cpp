#include "transform.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "UI/input.hpp"

#include "core/drawingBoard.hpp"

#include "utilities/maths.hpp"
#include "utilities/display.hpp"

#include "UI/log.hpp"

#include "graphics/immediateDrawing.hpp"

#include "UI/settings.hpp"

Transform::Transform() :
	m_translation(glm::vec2(0.0f)), m_scale(1.0f), m_rotation(0.0f),
	m_matrix(glm::mat4x4(1.0f)), m_inverseMatrix(glm::mat4x4(1.0f)), m_uAxis(glm::vec2(1.0f, 0.0f)), m_vAxis(glm::vec2(0.0f, 1.0f)),
	bMatrixMustBeRecomputed(false), bInverseMatrixMustBeRecomputed(false), bAxesMustBeRecomputed(false),
	bDraggingTranslation(false), bDraggingAltOrigin(false), bDraggingScale(false), m_mousePosWhenDraggingStarted(glm::vec2(0.0f)), m_translationWhenDraggingStarted(glm::vec2(0.0f)), m_altOriginInTransformSpaceWhenDraggingStarted(glm::vec2(0.0f)),
	m_altOriginInTransformSpace(glm::vec2(0.0f)), m_initialDragCenterInTransformSpace(glm::vec2(0.0f)), m_dragCenterInDrawingBoardSpace(glm::vec2(0.0f)), m_dragCenterInWindowSpace(glm::vec2(0.0f)),
	m_mouseRelPosWhenDraggingStartedInWindowSpace(glm::vec2(0.0f)), m_invDistToDragCenterSqWhenDraggingStartedinWindowSpace(0.0f), m_scaleWhenDraggingStarted(1.0f), m_matrixWhenDraggingStarted(glm::mat4x4(1.0f)),
	bDraggingRotation(false), m_rotationWhenDraggingStarted(0.0f)
{
}

Transform::~Transform() {

}

glm::vec2 Transform::getAltOriginInWindowSpace() { 
	return DrawingBoard::transform.getMatrix() * getMatrix() * glm::vec4(getAltOriginInTransformSpace(), 0.0f, 1.0f);
}
glm::vec2 Transform::getAltOriginInDrawingBoardSpace() {
	return getMatrix() * glm::vec4(getAltOriginInTransformSpace(), 0.0f, 1.0f);
}

void Transform::showAltOrigin() {
	//float defaultScale = getScale();
	//float dbScale = DrawingBoard::transform.getScale();
	//setScale(std::exp(Maths::clamp(std::log(defaultScale), -0.5f, 0.5f)));
	//DrawingBoard::transform.setScale(std::exp(Maths::clamp(std::log(dbScale), -0.5f, 0.5f)));
	ImmediateDrawing::setViewProjMatrix(Display::getProjMat() * DrawingBoard::transform.getMatrix() * getMatrix());
	//setScale(defaultScale);
	//DrawingBoard::transform.setScale(dbScale);
	float scale = getScale()* DrawingBoard::transform.getScale();
	glm::vec2 altOrig = getAltOriginInTransformSpace();
	ImmediateDrawing::setColor(0.0f, 0.0f, 0.0f, 0.5f);
	ImmediateDrawing::ring(altOrig.x, altOrig.y, 0.0f, Settings::ALT_ORIGIN_RADIUS / scale);
	ImmediateDrawing::setColor(1.0f, 1.0f, 1.0f, 0.5f);
	ImmediateDrawing::ring(altOrig.x, altOrig.y, Settings::ALT_ORIGIN_RADIUS / scale, (Settings::ALT_ORIGIN_RADIUS*1.5f) / scale);
	ImmediateDrawing::setColor(0.8f, 0.8f, 0.8f, 0.5f);
	ImmediateDrawing::ring(altOrig.x, altOrig.y, Settings::START_ROTATING_MIN_RADIUS / scale, Settings::START_ROTATING_MAX_RADIUS / scale);
}

void Transform::startDraggingTranslation() {
	if (!bDraggingTranslation) {
		bDraggingTranslation = true;
		m_mousePosWhenDraggingStarted = Input::getMousePosition();
		m_translationWhenDraggingStarted = m_translation;
	}
}
void Transform::startDraggingAltOrigin() {
	if (!bDraggingAltOrigin) {
		bDraggingAltOrigin = true;
		m_mousePosWhenDraggingStarted = Input::getMousePosition();
		m_altOriginInTransformSpaceWhenDraggingStarted = getAltOriginInTransformSpace();
	}
}
void Transform::startDraggingScale(glm::vec2 scaleOriginInTransformSpace) {
	if (!bDraggingScale) {
		bDraggingScale = true;

		m_matrixWhenDraggingStarted = getMatrix();
		m_mousePosWhenDraggingStarted = Input::getMousePosition();
		m_initialDragCenterInTransformSpace = scaleOriginInTransformSpace;

		m_dragCenterInDrawingBoardSpace = getMatrix() * glm::vec4(scaleOriginInTransformSpace, 0.0f, 1.0f);
		m_dragCenterInWindowSpace = DrawingBoard::transform.getMatrix() * glm::vec4(m_dragCenterInDrawingBoardSpace, 0.0f, 1.0f);
		m_mouseRelPosWhenDraggingStartedInWindowSpace = Input::getMousePosition() - m_dragCenterInWindowSpace;
		m_invDistToDragCenterSqWhenDraggingStartedinWindowSpace = 1.0f / glm::length2(m_mouseRelPosWhenDraggingStartedInWindowSpace);
		m_scaleWhenDraggingStarted = m_scale;
		m_translationWhenDraggingStarted = m_translation;
	}
}
void Transform::startDraggingRotation(glm::vec2 rotationOriginInDBspace) {
	if (!bDraggingRotation) {
		bDraggingRotation = true;
		m_mouseRelPosWhenDraggingStartedInWindowSpace = glm::vec4(Input::getMousePosition(), 0.0f, 0.0f) - DrawingBoard::transform.getMatrix() * glm::vec4(rotationOriginInDBspace, 0.0f, 1.0f);
		m_rotationWhenDraggingStarted = m_rotation;
		m_translationWhenDraggingStarted = m_translation;
		m_dragCenterInDrawingBoardSpace = rotationOriginInDBspace;
		m_dragCenterInWindowSpace = DrawingBoard::transform.getMatrix() * glm::vec4(rotationOriginInDBspace, 0.0f, 1.0f);
	}
}
void Transform::startDraggingRotation() {
	startDraggingRotation(getAltOriginInDrawingBoardSpace());
}
void Transform::changeDraggingScaleOrigin(glm::vec2 newScaleOriginInTransformSpace) {
	if (bDraggingScale) {
		m_dragCenterInDrawingBoardSpace = m_matrixWhenDraggingStarted * glm::vec4(newScaleOriginInTransformSpace, 0.0f, 1.0f);
		m_dragCenterInWindowSpace = DrawingBoard::transform.getMatrix() * glm::vec4(m_dragCenterInDrawingBoardSpace, 0.0f, 1.0f);
		m_mouseRelPosWhenDraggingStartedInWindowSpace = m_mousePosWhenDraggingStarted - m_dragCenterInWindowSpace;
		m_invDistToDragCenterSqWhenDraggingStartedinWindowSpace = 1.0f / glm::length2(m_mouseRelPosWhenDraggingStartedInWindowSpace);
	}
}
void Transform::revertDraggingScaleToInitialOrigin() {
	changeDraggingScaleOrigin(m_initialDragCenterInTransformSpace);
}
void Transform::changeDraggingScaleToAltOrigin() {
	changeDraggingScaleOrigin(getAltOriginInTransformSpace());
}
void Transform::checkDragging() {
	if (bDraggingTranslation) {
		glm::vec4 dl = DrawingBoard::transform.getInverseMatrix() * glm::vec4(Input::getMousePosition() - m_mousePosWhenDraggingStarted, 0.0f, 0.0f);
		setTranslation(m_translationWhenDraggingStarted + glm::vec2(dl.x, dl.y));
	}
	if (bDraggingAltOrigin) {
		glm::vec4 dl = getInverseMatrix() * DrawingBoard::transform.getInverseMatrix() * glm::vec4(Input::getMousePosition() - m_mousePosWhenDraggingStarted, 0.0f, 0.0f);
		m_altOriginInTransformSpace = m_altOriginInTransformSpaceWhenDraggingStarted + glm::vec2(dl.x, dl.y);
	}
	if (bDraggingScale) {
		glm::vec2 mousePosRelToScaleOriginInWindowSpace = Input::getMousePosition() - m_dragCenterInWindowSpace;
		float scaleFactor = glm::dot(mousePosRelToScaleOriginInWindowSpace, m_mouseRelPosWhenDraggingStartedInWindowSpace) * m_invDistToDragCenterSqWhenDraggingStartedinWindowSpace;

		setTranslation(scaleFactor * m_translationWhenDraggingStarted + (1 - scaleFactor) * m_dragCenterInDrawingBoardSpace);
		setScale(m_scaleWhenDraggingStarted * scaleFactor);
	}
	if (bDraggingRotation) {
		glm::vec2 mouseRelPos = Input::getMousePosition() - m_dragCenterInWindowSpace;
		float angle = glm::orientedAngle(glm::normalize(m_mouseRelPosWhenDraggingStartedInWindowSpace), glm::normalize(mouseRelPos));

		setRotation(m_rotationWhenDraggingStarted);
		setTranslation(m_translationWhenDraggingStarted);
		rotate(angle, m_dragCenterInDrawingBoardSpace);
	}
}
bool Transform::endDragging() {
	bool handled = bDraggingTranslation || bDraggingAltOrigin || bDraggingScale || bDraggingRotation;
	bDraggingTranslation = false;
	bDraggingAltOrigin = false;
	bDraggingScale = false;
	bDraggingRotation = false;
	return handled;
}

void Transform::setMatrix(glm::mat4x4 matrix) {
	m_matrix = matrix;
	bInverseMatrixMustBeRecomputed = true;
}

const glm::mat4x4& Transform::getMatrix() {
	if (bMatrixMustBeRecomputed)
		computeMatrix();
	return m_matrix;
}

const glm::mat4x4& Transform::getInverseMatrix() {
	if (bInverseMatrixMustBeRecomputed)
		computeInverseMatrix();
	return m_inverseMatrix;
}

const glm::vec2& Transform::getUAxis() {
	if (bAxesMustBeRecomputed)
		computeAxes();
	return m_uAxis;
}

const glm::vec2& Transform::getVAxis() {
	if (bAxesMustBeRecomputed)
		computeAxes();
	return m_vAxis;
}

void Transform::setTranslation(glm::vec2 translation) {
	m_translation = translation;
	bMatrixMustBeRecomputed = true;
	bInverseMatrixMustBeRecomputed = true;
}
void Transform::translate(glm::vec2 translation) {
	setTranslation(m_translation + translation);
}
void Transform::setScale(float scale) {
	m_scale = scale;
	bMatrixMustBeRecomputed = true;
	bInverseMatrixMustBeRecomputed = true;
}
void Transform::scale(float scale) {
	setScale(m_scale * scale);
}
void Transform::scale(float scale, glm::vec2 originInDBspace) {
	setTranslation(scale * m_translation + (1 - scale) * originInDBspace);
	setScale(m_scale * scale);
}
void Transform::setRotation(float rotation) {
	m_rotation = rotation;
	bMatrixMustBeRecomputed = true;
	bInverseMatrixMustBeRecomputed = true;
	bAxesMustBeRecomputed = true;
}
void Transform::rotate(float rotation) {
	setRotation(m_rotation + rotation);
}
void Transform::rotate(float rotation, glm::vec2 origin) {
	setTranslation(Maths::rotate(m_translation-origin,rotation) + origin);
	setRotation(m_rotation + rotation);
}
void Transform::computeMatrix() {
	if (!bInverseMatrixMustBeRecomputed) {
		m_matrix = glm::inverse(m_inverseMatrix);
	}
	else {
		m_matrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(m_translation, 0.0f));
		m_matrix = glm::rotate(m_matrix, m_rotation, glm::vec3(0.0, 0.0, 1.0));
		m_matrix = glm::scale(m_matrix, glm::vec3(m_scale, m_scale, 1.0f));
	}
	bMatrixMustBeRecomputed = false;
}
void Transform::computeInverseMatrix() {
	if (!bMatrixMustBeRecomputed) {
		m_inverseMatrix = glm::inverse(m_matrix);
	}
	else {
		m_inverseMatrix = glm::scale(glm::mat4x4(1.0f), glm::vec3(1.0f / m_scale, 1.0f / m_scale, 1.0f));
		m_inverseMatrix = glm::rotate(m_inverseMatrix, -m_rotation, glm::vec3(0.0, 0.0, 1.0));
		m_inverseMatrix = glm::translate(m_inverseMatrix, -glm::vec3(m_translation, 0.0f));
	}
	bInverseMatrixMustBeRecomputed = false;
}
void Transform::computeAxes() {
	float c = cos(getRotation());
	float s = sin(getRotation());
	m_uAxis = glm::vec2( c, s);
	m_vAxis = glm::vec2(-s, c);
	bAxesMustBeRecomputed = false;
}

void Transform::reset() {
	setTranslation(glm::vec2(0.0f));
	setScale(1.0f);
	setRotation(0.0f);
}