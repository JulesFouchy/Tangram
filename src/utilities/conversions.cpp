#include "conversions.hpp"

#include "display.hpp"
#include "maths.hpp"

glm::vec2 conv::screenCoordFromPixelCoord(int x, int y) {
	return glm::vec2(
		Maths::map(x, 0.0f, Display::getWidth(), Display::getMinX(), Display::getMaxX()),
		Maths::map(y, Display::getHeight(), 0.0f, Display::getMinY(), Display::getMaxY())
	);
}