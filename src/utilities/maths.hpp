#pragma once

#include "glm/glm.hpp"

namespace Maths {
	const float TAU = 6.28318530717958647692;

	float map(float x, float a0, float b0, float a1, float b1);
	float clamp(float x, float a, float b);

	glm::vec2 rotate(glm::vec2 vec, float angle);
}