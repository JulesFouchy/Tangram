#pragma once

#include "glm/glm.hpp"
#include "SDL.h"

class Display {
public:
	static glm::vec2 getSize();
	static float getWidth();
	static float getHeight();
	static float getRatio();
	static float getMinX();
	static float getMaxX();
	static float getMinY();
	static float getMaxY();
public:
	static void onWindowResized();

public:
	Display() = delete;
	static void initialize(SDL_Window* window);
	~Display() = delete;

private:
	static SDL_Window* s_window;
	static glm::vec2 s_windowSize;
	static float s_ratio;
	static float s_minX;
	static float s_maxX;
	static float s_minY; 
	static float s_maxY; 
private:
	static void s_updateWindowSize();
};