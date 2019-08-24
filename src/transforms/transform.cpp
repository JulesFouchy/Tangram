#include "transform.hpp"

Transform::Transform() :
	m_matrix(glm::mat4x4(1.0f)), m_inverseMatrix(glm::mat4x4(1.0f)),
	bMatrixMustBeRecomputed(false), bInverseMatrixMustBeRecomputed(false)
{
}


void Transform::setMatrix(glm::mat4x4 matrix) {
	m_matrix = matrix;
	bInverseMatrixMustBeRecomputed = true;
}

const glm::mat4x4& Transform::getMatrix() {
	if (bMatrixMustBeRecomputed)
		computeMatrix();
	return m_matrix;
}

const glm::mat4x4& Transform::getInverseMatrix() {
	if (bInverseMatrixMustBeRecomputed)
		computeInverseMatrix();
	return m_inverseMatrix;
}