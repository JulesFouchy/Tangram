#include "inputHandler.hpp"

#include "spdlog/spdlog.h"

InputHandler::InputHandler()
	: m_mousePos(glm::vec2(0.0f)), m_leftClicIsDown(false), m_spaceBarIsDown(false), m_mousePosWhenLeftClicDown(glm::vec2(0.0f))
{

}

InputHandler::~InputHandler() {

}

glm::vec2 InputHandler::getMousePosition() {
	return m_mousePos;
}
bool InputHandler::leftClicIsDown() {
	return m_leftClicIsDown;
}
bool InputHandler::spaceBarIsDown() {
	return m_spaceBarIsDown;
}
glm::vec2 InputHandler::getMousePosWhenLeftClicDown(){
	return m_mousePosWhenLeftClicDown;
}

void InputHandler::onMouseMove(glm::vec2 mousePos) {
	m_mousePos = mousePos;
}
void InputHandler::onLeftClicDown(glm::vec2 mousePos) {
	m_leftClicIsDown = true;
	m_mousePosWhenLeftClicDown = mousePos;
}
void InputHandler::onLeftClicUp() {
	m_leftClicIsDown = false;
}
void InputHandler::onStandardKeyDown(char key) {
	if (key == ' ') {
		m_spaceBarIsDown = true;
	}
}
void InputHandler::onStandardKeyUp(char key) {
	if (key == ' ') {
		m_spaceBarIsDown = false;
	}
}