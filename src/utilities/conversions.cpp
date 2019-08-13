#include "conversions.hpp"

#include "constants.hpp"

glm::vec2 conv::screenCoordFromPixelCoord(int x, int y) {
	return glm::vec2(x / WIN_HEIGHT, 1 - y / WIN_HEIGHT);
}