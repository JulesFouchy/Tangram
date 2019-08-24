#pragma once

#include "glm/glm.hpp"

class Transform {
public:
	const glm::mat4x4& getMatrix();
	const glm::mat4x4& getInverseMatrix();
	void setMatrix(glm::mat4x4 matrix);
public:
	virtual void onLeftClicDown() = 0;
	virtual void onSpaceBarDown() = 0;
	virtual void onLeftClicUp() = 0;
	virtual void onSpaceBarUp() = 0;
protected:
	Transform();
protected:
	glm::mat4x4 m_matrix;
	glm::mat4x4 m_inverseMatrix;
	bool bMatrixMustBeRecomputed;
	bool bInverseMatrixMustBeRecomputed;
	virtual void computeMatrix() = 0;
	virtual void computeInverseMatrix() = 0;
};