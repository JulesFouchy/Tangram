#include "rectTransform.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "UI/input.hpp"

RectTransform::RectTransform(float aspectRatio)
	: m_aspectRatio(aspectRatio), m_translation(glm::vec2(0.0f)), m_scale(1.0f), m_rotation(0.0f),
	bDraggingTranslation(false), m_mousePosWhenDraggingStarted(glm::vec2(0.0f)), m_translationWhenDraggingStarted(glm::vec2(0.0f))
{
	computeTransformMatrix();
}
RectTransform::~RectTransform(){
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


void RectTransform::checkInputs() {
	checkDraggingTranslation();
}

void RectTransform::checkDraggingTranslation() {
	if (bDraggingTranslation) {
		setTranslation(m_translationWhenDraggingStarted + Input::getMousePosition() - m_mousePosWhenDraggingStarted);
	}
}
void RectTransform::startDraggingTranslation() {
	bDraggingTranslation = true;
	m_mousePosWhenDraggingStarted = Input::getMousePosition();
	m_translationWhenDraggingStarted = m_translation;
}
void RectTransform::endDraggingTranslation() {
	bDraggingTranslation = false;
}

void RectTransform::onLeftClicDown() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown()) {
		startDraggingTranslation();
	}
}
void RectTransform::onSpaceBarDown() {
	//Moving by holding space + clic'n'dragging
	if (Input::leftClicIsDown()) {
		startDraggingTranslation();
	}
}
void RectTransform::onLeftClicUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::spaceBarIsDown()) {
		endDraggingTranslation();
	}
}
void RectTransform::onSpaceBarUp() {
	//Moving by holding space + clic'n'dragging
	if (Input::leftClicIsDown()) {
		endDraggingTranslation();
	}
}