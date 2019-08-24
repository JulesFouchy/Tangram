#pragma once

#include "transform.hpp"

class RectTransform : public Transform {
public:
	RectTransform(float aspectRatio);
	~RectTransform();

	inline float getAspectRatio() { return m_aspectRatio; };

	void checkInputs();
	void onLeftClicDown() override;
	void onSpaceBarDown() override;
	void onLeftClicUp() override;
	void onSpaceBarUp() override;

	void startDraggingTranslation();
	void endDraggingTranslation();

	void setTranslation(glm::vec2 translation);
	void translate(glm::vec2 translation);
	void setScale(float scale);
	void scale(float scale);
	void scale(float scale, glm::vec2 origin);
	void setRotation(float rotation);
	void rotate(float rotation);
	void reset();

private:
	float m_aspectRatio;
	glm::vec2 m_translation;
	float m_scale;
	float m_rotation;
	void computeTransformMatrix();
protected:
	bool bDraggingTranslation;
	glm::vec2 m_mousePosWhenDraggingStarted;
	glm::vec2 m_translationWhenDraggingStarted;
	void checkDraggingTranslation();
};