#include "display.hpp"

#include "spdlog/spdlog.h"

SDL_Window* Display::s_window;
glm::vec2 Display::s_windowSize;
float Display::s_ratio;
float Display::s_minX;
float Display::s_maxX;
float Display::s_minY;
float Display::s_maxY;

void Display::initialize(SDL_Window* window)
{
	s_window = window;
	s_updateWindowSize();
	s_minY = -0.5;
	s_maxY =  0.5;
}

glm::vec2 Display::getSize() {
	return s_windowSize;
}
float Display::getWidth() {
	return s_windowSize.x;
}
float Display::getHeight() {
	return s_windowSize.y;
}
float Display::getRatio() {
	return s_ratio;
}
float Display::getMinX() {
	return s_minX;
}
float Display::getMaxX() {
	return s_maxX;
}
float Display::getMinY() {
	return s_minY;
}
float Display::getMaxY() {
	return s_maxY;
}

void Display::s_updateWindowSize() {
	int w, h;
	SDL_GetWindowSize(s_window, &w, &h);
	s_windowSize = glm::vec2(w, h);
	s_ratio = s_windowSize.x / s_windowSize.y;
	s_minX = -0.5 * s_ratio;
	s_maxX =  0.5 * s_ratio;
}

void Display::onWindowResized() {
	s_updateWindowSize();
	spdlog::info("[Window Resized] {} {}", getWidth(), getHeight());
}