#include "controls.hpp"
#include "input.hpp"

bool Controls::isRotationSegmented() {
	return !Input::keyIsDown(SHIFT);
}

bool Controls::draggingScaleRespectsAspectRatio() {
	return !Input::keyIsDown(SHIFT);
}

bool Controls::dragTranslationAlongOnlyOneAxis() {
	return Input::keyIsDown(SHIFT);
}