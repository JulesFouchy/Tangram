#pragma once

#include "transform.hpp"

class RectTransform : public Transform {
public:
	RectTransform(float aspectRatio);
	~RectTransform();

	inline float getAspectRatio() { return m_aspectRatio; };

	bool mouseIsHovering();
	bool mouseIsHovering(glm::mat4x4 viewMatrix);

	void checkInputs(glm::mat4x4 inverseViewMatrix = glm::mat4x4(1.0f));
	void onLeftClicDown() override;
	void onLeftClicDown(glm::mat4x4 viewTransform);
	void onSpaceBarDown() override;
	void onLeftClicUp() override;
	void onSpaceBarUp() override;

private:
	float m_aspectRatio;
};