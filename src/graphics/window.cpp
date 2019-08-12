#include "window.hpp"

#include <iostream>

void MyWindow::clearScreen() {
	m_clearScreenShader.bind();
	glBindBuffer(GL_ARRAY_BUFFER, m_clearScreenVbId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_clearScreenIboId);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void MyWindow::swapBuffers() {
	// Swap front and back buffers
	glfwSwapBuffers(windowPtr);
}

void MyWindow::pollEvents(){
	// Poll for and process events 
	glfwPollEvents();
}

bool MyWindow::windowShouldClose() {
	return glfwWindowShouldClose(windowPtr);
}

MyWindow::MyWindow(GLFWwindow* windowPtr)
	: windowPtr(windowPtr), m_clearScreenShader("res/shaders/clearscreen/basic.vert", "res/shaders/clearscreen/basic.frag")
{
	//To clear the screen
	//Gen vertex buffer for rendering on screen
	float vertices[] = {
		//Position
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f,
		1.0f, -1.0f,
	};
	glGenBuffers(1, &m_clearScreenVbId);
	glBindBuffer(GL_ARRAY_BUFFER, m_clearScreenVbId);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Gen index buffer
	unsigned int indices[] = {
		0, 2, 1,
		0, 3, 2
	};
	glGenBuffers(1, &m_clearScreenIboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_clearScreenIboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

MyWindow::~MyWindow() {
	shutDownImGui();
	glfwTerminate();
}