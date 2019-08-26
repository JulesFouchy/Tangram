#pragma once

#include "glm/glm.hpp"

class Transform {
public:
	const glm::mat4x4& getMatrix();
	const glm::mat4x4& getInverseMatrix();
	void setMatrix(glm::mat4x4 matrix);
public:

	void startDraggingTranslation();
	void startDraggingScale();
	void checkDragging();
	bool endDragging();

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
	bool bDraggingScale;
	glm::vec2 m_mousePosWhenDraggingStarted;
	glm::vec2 m_translationWhenDraggingStarted;
	float m_scaleWhenDraggingStarted;
protected:
	Transform();
};