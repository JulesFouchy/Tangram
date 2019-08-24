#include "rectTransform.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "UI/input.hpp"

#include "spdlog/spdlog.h"

#include <algorithm>

#include "glm/gtc/matrix_inverse.hpp"
RectTransform::RectTransform(float aspectRatio)
	: m_aspectRatio(aspectRatio), m_translation(glm::vec2(0.0f)), m_scale(1.0f), m_rotation(0.0f),
	bDraggingTranslation(false), m_mousePosWhenDraggingStarted(glm::vec2(0.0f)), m_translationWhenDraggingStarted(glm::vec2(0.0f))
{
	computeTransformMatrix();
}
RectTransform::~RectTransform(){
}

bool RectTransform::mouseIsHovering() {
	return mouseIsHovering(glm::mat4x4(1.0f));
}
bool RectTransform::mouseIsHovering(glm::mat4x4 viewTransform) {
	glm::vec4 dl = glm::inverse(viewTransform * glm::scale(getMatrix(), glm::vec3(getAspectRatio(), 1.0f, 1.0f))) * glm::vec4(Input::getMousePosition(), 0.0f, 1.0f);
	return std::max(abs(dl.x), abs(dl.y)) < 0.5f;
}

void RectTransform::setTranslation(glm::vec2 translation) {
	m_translation = translation;
	computeTransformMatrix();
}
void RectTransform::translate(glm::vec2 translation) {
	setTranslation(m_translation + translation);
}
void RectTransform::setScale(float scale) {
	m_scale = scale;
	computeTransformMatrix();
}
void RectTransform::scale(float scale) {
	setScale(m_scale * scale);
}
void RectTransform::scale(float scale, glm::vec2 origin) {
	setTranslation(scale * m_translation + (1 - scale) * origin);
	setScale(m_scale * scale);
}
void RectTransform::setRotation(float rotation) {
	m_rotation = rotation;
	computeTransformMatrix();
}
void RectTransform::rotate(float rotation) {
	setRotation(m_rotation + rotation);
}
void RectTransform::computeTransformMatrix() {
	m_matrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(m_translation, 0.0f));
	m_matrix = glm::rotate(m_matrix, m_rotation, glm::vec3(0.0, 0.0, 1.0));
	m_matrix = glm::scale(m_matrix, glm::vec3(m_scale, m_scale, 1.0f));
}
void RectTransform::reset() {
	setTranslation(glm::vec2(0.0f));
	setScale(1.0f);
	setRotation(0.0f);
}
void RectTransform::setMatrix(glm::mat4x4 matrix) {
	m_matrix = matrix;
}


void RectTransform::checkInputs(glm::mat4x4 viewMatrix) {
	checkDraggingTranslation(viewMatrix);
}

void RectTransform::checkDraggingTranslation(glm::mat4x4 viewMatrix) {
	if (bDraggingTranslation) {
		glm::vec4 dl = glm::inverse(viewMatrix) * glm::vec4(Input::getMousePosition() - m_mousePosWhenDraggingStarted,0.0f,0.0f);
		setTranslation(m_translationWhenDraggingStarted + glm::vec2(dl.x,dl.y));
	}
}
void RectTransform::startDraggingTranslation() {
	if (!bDraggingTranslation) {
		bDraggingTranslation = true;
		m_mousePosWhenDraggingStarted = Input::getMousePosition();
		m_translationWhenDraggingStarted = m_translation;
	}
}
void RectTransform::endDraggingTranslation() {
	bDraggingTranslation = false;
}

void RectTransform::onLeftClicDown() {
	if (mouseIsHovering())
		startDraggingTranslation();
}
void RectTransform::onLeftClicDown(glm::mat4x4 viewTransform) {
	if (mouseIsHovering(viewTransform))
		startDraggingTranslation();
}
void RectTransform::onSpaceBarDown() {
}
void RectTransform::onLeftClicUp() {
	endDraggingTranslation();
}
void RectTransform::onSpaceBarUp() {
}