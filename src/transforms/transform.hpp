#pragma once

#include "glm/glm.hpp"

class Transform {
public:
	inline const glm::mat4x4& getMatrix() {return m_matrix;};
public:
	virtual void onLeftClicDown() = 0;
	virtual void onSpaceBarDown() = 0;
	virtual void onLeftClicUp() = 0;
	virtual void onSpaceBarUp() = 0;
protected:
	inline Transform() : m_matrix(glm::mat4x4(1.0f)) {}
protected:
	glm::mat4x4 m_matrix;
};