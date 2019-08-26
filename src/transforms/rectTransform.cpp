#include "rectTransform.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "UI/input.hpp"

#include "UI/log.hpp"

#include <algorithm>

#include "glm/gtc/matrix_inverse.hpp"
RectTransform::RectTransform(float aspectRatio)
	: m_aspectRatio(aspectRatio), m_projectionMatrix(glm::ortho(-0.5f * m_aspectRatio, 0.5f * m_aspectRatio, -0.5f, 0.5f))
{
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

void RectTransform::checkInputs(glm::mat4x4 inverseViewMatrix) {
	checkDraggingTranslation(inverseViewMatrix);
}

bool RectTransform::onLeftClicDown() {
	if (mouseIsHovering()) {
		startDraggingTranslation();
		return true;
	}
	return false;
}
bool RectTransform::onLeftClicDown(glm::mat4x4 viewTransform) {
	if (mouseIsHovering(viewTransform)) {
		startDraggingTranslation();
		return true;
	}
	return false;
}
bool RectTransform::onSpaceBarDown() {
	return false;
}
bool RectTransform::onLeftClicUp() {
	bool handled = bDraggingTranslation;
	endDraggingTranslation();
	return handled;
}
bool RectTransform::onSpaceBarUp() {
	return false;
}