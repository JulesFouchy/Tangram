#include "input.hpp"

#include "spdlog/spdlog.h"

glm::vec2 Input::m_mousePos;
bool Input::m_leftClicIsDown;
bool Input::m_spaceBarIsDown;
glm::vec2 Input::m_mousePosWhenLeftClicAndSpaceBarDown;

void Input::initialize() {
	m_mousePos = glm::vec2(0.0f);
	m_leftClicIsDown = false;
	m_spaceBarIsDown = false;
	m_mousePosWhenLeftClicAndSpaceBarDown = glm::vec2(0.0f);
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
glm::vec2 Input::getMousePosWhenLeftClicAndSpaceBarDown(){
	return m_mousePosWhenLeftClicAndSpaceBarDown;
}

void Input::onMouseMove(glm::vec2 mousePos) {
	m_mousePos = mousePos;
}
void Input::onLeftClicDown(glm::vec2 mousePos) {
	m_leftClicIsDown = true;
	if (spaceBarIsDown()) {
		m_mousePosWhenLeftClicAndSpaceBarDown = mousePos;
	}
}
void Input::onLeftClicUp() {
	m_leftClicIsDown = false;
}
void Input::onStandardKeyDown(char key) {
	if (key == ' ') {
		if (!m_spaceBarIsDown && leftClicIsDown()) {
			m_mousePosWhenLeftClicAndSpaceBarDown = m_mousePos;
		}
		m_spaceBarIsDown = true;
	}
}
void Input::onStandardKeyUp(char key) {
	if (key == ' ') {
		m_spaceBarIsDown = false;
	}
}