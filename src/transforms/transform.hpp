#pragma once

#include "glm/glm.hpp"

class Transform {
public:
	const glm::mat4x4& getMatrix();
	const glm::mat4x4& getInverseMatrix();
	void setMatrix(glm::mat4x4 matrix);
public:
	virtual bool onLeftClicDown() = 0;
	virtual bool onSpaceBarDown() = 0;
	virtual bool onLeftClicUp() = 0;
	virtual bool onSpaceBarUp() = 0;

	void startDraggingTranslation();
	void endDraggingTranslation();

	void setTranslation(glm::vec2 translation);
	void translate(glm::vec2 translation);
	inline glm::vec2 getTranslation() { return m_translation; }
	void setScale(float scale);
	void scale(float scale);
	void scale(float scale, glm::vec2 origin);
	inline float getScale() { return m_scale; };
	void setRotation(float rotation);
	void rotate(float rotation);
	inline float getRotation() { return m_rotation; }
	void reset();
protected:
	Transform();
private:
	glm::vec2 m_translation;
	float m_scale;
	float m_rotation;
protected:
	glm::mat4x4 m_matrix;
	glm::mat4x4 m_inverseMatrix;
	bool bMatrixMustBeRecomputed;
	bool bInverseMatrixMustBeRecomputed;
	void computeMatrix();
	void computeInverseMatrix();

	bool bDraggingTranslation;
	glm::vec2 m_mousePosWhenDraggingStarted;
	glm::vec2 m_translationWhenDraggingStarted;
	void checkDraggingTranslation(glm::mat4x4 inverseViewMatrix = glm::mat4x4(1.0f));
};