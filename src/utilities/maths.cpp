#include "maths.hpp"

float Maths::map(float x, float a0, float b0, float a1, float b1) {
	return (x - a0)/(b0-a0)*(b1-a1) + a1;
}