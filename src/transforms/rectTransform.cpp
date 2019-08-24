#include "rectTransform.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "UI/input.hpp"

#include "spdlog/spdlog.h"

#include <algorithm>

#include "glm/gtc/matrix_inverse.hpp"
RectTransform::RectTransform(float aspectRatio)
	: m_aspectRatio(aspectRatio)
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