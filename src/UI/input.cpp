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
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key
		switch (*specialKey)
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
	spdlog::error("[Input::keyIsDown] char keys not supported yet");
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
	else {
		DrawingBoard::onLeftClicDown();
	}
}
void Input::onDoubleLeftClic() {
	DrawingBoard::onDoubleLeftClic();
}
void Input::onLeftClicUp() {
	m_leftClicIsDown = false;
	timeSinceLastLeftClic.reset();
	DrawingBoard::onLeftClicUp();
}
void Input::onKeyDown(Key key) {
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key

	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char
		if (*c == ' ') {
			m_spaceBarIsDown = true;
		}
	}
	DrawingBoard::onKeyDown(key);
}
void Input::onKeyUp(Key key) {
	if (auto specialKey = std::get_if<SpecialKey>(&key)) { //Check if it's a special key

	}
	else if (auto c = std::get_if<char>(&key)) { //It's a char
		if (*c == ' ') {
			m_spaceBarIsDown = false;
		}
	}
	DrawingBoard::onKeyUp(key);
}