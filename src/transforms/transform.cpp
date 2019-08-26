#include "transform.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "UI/input.hpp"

#include "core/drawingBoard.hpp"

#include "UI/log.hpp"

Transform::Transform() :
	m_translation(glm::vec2(0.0f)), m_scale(1.0f), m_rotation(0.0f),
	m_matrix(glm::mat4x4(1.0f)), m_inverseMatrix(glm::mat4x4(1.0f)),
	bMatrixMustBeRecomputed(false), bInverseMatrixMustBeRecomputed(false),
	bDraggingTranslation(false), bDraggingScale(false), m_mousePosWhenDraggingStarted(glm::vec2(0.0f)), m_translationWhenDraggingStarted(glm::vec2(0.0f)),
	m_scaleWhenDraggingStarted(1.0f)
{
}

void Transform::startDraggingTranslation() {
	if (!bDraggingTranslation) {
		bDraggingTranslation = true;
		m_mousePosWhenDraggingStarted = Input::getMousePosition();
		m_translationWhenDraggingStarted = m_translation;
	}
}
void Transform::startDraggingScale() {
	if (!bDraggingScale) {
		bDraggingScale = true;
		m_mousePosWhenDraggingStarted = Input::getMousePosition();
		m_scaleWhenDraggingStarted = m_scale;
	}
}
void Transform::checkDragging() {
	glm::vec4 dl = DrawingBoard::transform.getInverseMatrix() * glm::vec4(Input::getMousePosition() - m_mousePosWhenDraggingStarted, 0.0f, 0.0f);
	if (bDraggingTranslation) {
		setTranslation(m_translationWhenDraggingStarted + glm::vec2(dl.x, dl.y));
	}
	if (bDraggingScale) {
		setScale(0.5f);
	}
}
bool Transform::endDragging() {
	bool handled = bDraggingTranslation || bDraggingScale;
	bDraggingTranslation = false;
	bDraggingScale = false;
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
void Transform::scale(float scale, glm::vec2 origin) {
	setTranslation(scale * m_translation + (1 - scale) * origin);
	setScale(m_scale * scale);
}
void Transform::setRotation(float rotation) {
	m_rotation = rotation;
	bMatrixMustBeRecomputed = true;
	bInverseMatrixMustBeRecomputed = true;
}
void Transform::rotate(float rotation) {
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
void Transform::reset() {
	setTranslation(glm::vec2(0.0f));
	setScale(1.0f);
	setRotation(0.0f);
}