#include "maths.hpp"

#include <algorithm>

float Maths::map(float x, float a0, float b0, float a1, float b1) {
	return (x - a0)/(b0-a0)*(b1-a1) + a1;
}

float Maths::clamp(float x, float a, float b) {
	return std::min(std::max(x, a), b);
}

glm::vec2 Maths::rotate(glm::vec2 vec, float angle) {
	float cos = std::cos(angle);
	float sin = std::sin(angle);
	return glm::vec2(	cos * vec.x + -sin * vec.y,
						sin * vec.x +  cos * vec.y	
					);
}