#include "controls.hpp"
#include "input.hpp"

bool Controls::isRotationSegmented() {
	return !Input::keyIsDown(SHIFT);
}