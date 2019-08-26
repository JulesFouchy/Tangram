#pragma once

#include "transform.hpp"

enum MousePositionRelativeToRect {
	OUTSIDE,
	INSIDE,
	RIGHT,
	TOP_RIGHT,
	TOP,
	TOP_LEFT,
	LEFT,
	BOT_LEFT,
	BOT,
	BOT_RIGHT
};

class RectTransform : public Transform {
public:
	RectTransform(float aspectRatio);
	~RectTransform();

	inline float getAspectRatio() { return m_aspectRatio; };
	inline const glm::mat4x4& getProjectionMatrix() { return m_projectionMatrix; };

	MousePositionRelativeToRect getMouseRelativePosition();

	void checkInputs(glm::mat4x4 inverseViewMatrix = glm::mat4x4(1.0f));
	bool onLeftClicDown() override;
	bool onSpaceBarDown() override;
	bool onLeftClicUp() override;
	bool onSpaceBarUp() override;

private:
	float m_aspectRatio;
	glm::mat4x4 m_projectionMatrix;
};