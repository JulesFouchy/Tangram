#pragma once

#include "transform.hpp"

class RectTransform : public Transform {
public:
	RectTransform(float aspectRatio);
	~RectTransform();

	inline float getAspectRatio() { return m_aspectRatio; };
	inline const glm::mat4x4& getProjectionMatrix() { return m_projectionMatrix; };

	bool mouseIsHovering();
	bool mouseIsHovering(glm::mat4x4 viewMatrix);

	void checkInputs(glm::mat4x4 inverseViewMatrix = glm::mat4x4(1.0f));
	bool onLeftClicDown() override;
	bool onLeftClicDown(glm::mat4x4 viewTransform);
	bool onSpaceBarDown() override;
	bool onLeftClicUp() override;
	bool onSpaceBarUp() override;

private:
	float m_aspectRatio;
	glm::mat4x4 m_projectionMatrix;
};