#include "input.hpp"

#include "spdlog/spdlog.h"

#include "SDL.h"

#include "utilities/conversions.hpp"

bool Input::bDoubleLeftClic;
bool Input::m_leftClicIsDown;
bool Input::m_spaceBarIsDown;
glm::vec2 Input::m_mousePosWhenLastLeftClic;
Timestamp Input::timeSinceLastLeftClic;

void Input::initialize() {
	bDoubleLeftClic = false;
	m_leftClicIsDown = false;
	m_spaceBarIsDown = false;
	m_mousePosWhenLastLeftClic = glm::vec2(0.0f);
}

void Input::update() {
	bDoubleLeftClic = false;
}

Input::~Input() {

}

glm::vec2 Input::getMousePosition() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return conv::screenCoordFromPixelCoord(x, y);
}
bool Input::leftClicIsDown() {
	return m_leftClicIsDown;
}
bool Input::spaceBarIsDown() {
	return m_spaceBarIsDown;
}
glm::vec2 Input::getMousePosWhenLastLeftClic(){
	return m_mousePosWhenLastLeftClic;
}

void Input::onLeftClicDown(glm::vec2 mousePos) {
	m_leftClicIsDown = true;
	//Check for double clic
	if (timeSinceLastLeftClic.getAge() < 0.1f
		&& glm::length(m_mousePosWhenLastLeftClic-mousePos) < 0.05f) {
		onDoubleLeftClic(mousePos);
	}
	m_mousePosWhenLastLeftClic = mousePos;
}
void Input::onDoubleLeftClic(glm::vec2 mousePos) {
	bDoubleLeftClic = true;
}
void Input::onLeftClicUp() {
	m_leftClicIsDown = false;
	timeSinceLastLeftClic.reset();
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