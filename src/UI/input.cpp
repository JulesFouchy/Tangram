#include "inputHandler.hpp"

#include "spdlog/spdlog.h"

glm::vec2 Input::m_mousePos;
bool Input::m_leftClicIsDown;
bool Input::m_spaceBarIsDown;
glm::vec2 Input::m_mousePosWhenLeftClicDown;

void Input::initialize() {
	m_mousePos = glm::vec2(0.0f);
	m_leftClicIsDown = false;
	m_spaceBarIsDown = false;
	m_mousePosWhenLeftClicDown = glm::vec2(0.0f);
}

Input::~Input() {

}

glm::vec2 Input::getMousePosition() {
	return m_mousePos;
}
bool Input::leftClicIsDown() {
	return m_leftClicIsDown;
}
bool Input::spaceBarIsDown() {
	return m_spaceBarIsDown;
}
glm::vec2 Input::getMousePosWhenLeftClicDown(){
	return m_mousePosWhenLeftClicDown;
}

void Input::onMouseMove(glm::vec2 mousePos) {
	m_mousePos = mousePos;
}
void Input::onLeftClicDown(glm::vec2 mousePos) {
	m_leftClicIsDown = true;
	m_mousePosWhenLeftClicDown = mousePos;
}
void Input::onLeftClicUp() {
	m_leftClicIsDown = false;
}
void Input::onStandardKeyDown(char key) {
	if (key == ' ') {
		m_spaceBarIsDown = true;
	}
}
void Input::onStandardKeyUp(char key) {
	if (key == ' ') {
		m_spaceBarIsDown = false;
	}
}