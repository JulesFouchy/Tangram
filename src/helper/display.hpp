#pragma once

#include "SDL.h"

#include "glm/glm.hpp"

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
	static glm::mat4x4 getProjMat();
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
	static glm::mat4x4 s_projMat;
private:
	static void s_updateWindowSize();
	static void s_updateProjectionMatrix();
};