#include "input.hpp"

#include "UI/log.hpp"

#include "SDL.h"

#include "utilities/conversions.hpp"

#include "core/drawingBoard.hpp"

bool Input::m_leftClicIsDown;
bool Input::m_spaceBarIsDown;
glm::vec2 Input::m_mousePosWhenLastLeftClic;
Timestamp Input::timeSinceLastLeftClic;

void Input::Initialize() {
	m_leftClicIsDown = false;
	m_spaceBarIsDown = false;
	m_mousePosWhenLastLeftClic = glm::vec2(0.0f);
}

void Input::update() {
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
bool Input::keyIsDown(Key key) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	switch (key)
	{
	case ALT:
		return state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT];
		break;
	case CTRL:
		return state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL];
		break;
	case SHIFT:
		return state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT];
		break;
	default:
		spdlog::error("[Input::keyIsDown] reached default case");
		return false;
		break;
	}
}
glm::vec2 Input::getMousePosWhenLastLeftClic(){
	return m_mousePosWhenLastLeftClic;
}

void Input::onLeftClicDown() {
	m_leftClicIsDown = true;
	glm::vec2 mousePos = getMousePosition();
	m_mousePosWhenLastLeftClic = mousePos;
	//Check for double clic
	if (timeSinceLastLeftClic.getAge() < 0.1f && glm::length(m_mousePosWhenLastLeftClic-mousePos) < 0.05f){
		onDoubleLeftClic();
	}
}
void Input::onDoubleLeftClic() {
	DrawingBoard::onDoubleLeftClic();
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