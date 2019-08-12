#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "core/constants.hpp"

#include "graphics/shader.hpp"

class MyWindow {
public :
	MyWindow(GLFWwindow* windowPtr);
	~MyWindow();
	void clearScreen();
	void swapBuffers();
	void pollEvents();
	bool windowShouldClose();
	void shutDown();
	//glm::vec2 getNormalizedMousePosition();
	void initializeDearImGui();
	void startRenderImGui();
	void endRenderImGui();
	void shutDownImGui();

	bool mouseIsPressed;

	GLFWwindow* windowPtr;
private:
	unsigned int m_clearScreenVbId;
	unsigned int m_clearScreenIboId;
	Shader m_clearScreenShader;
};