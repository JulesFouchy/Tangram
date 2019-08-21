#include "conversions.hpp"

#include "utilities/display.hpp"

glm::vec2 conv::screenCoordFromPixelCoord(int x, int y) {
	return glm::vec2(x / Display::getHeight(), 1 - y / Display::getHeight());
}