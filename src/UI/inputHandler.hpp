#pragma once

#include "glm/glm.hpp"

class InputHandler {

public:
	InputHandler();
	~InputHandler();

	glm::vec2 getMousePosition();
	bool leftClicIsDown();
	bool spaceBarIsDown();
	glm::vec2 getMousePosWhenLeftClicDown();

	void onMouseMove(glm::vec2 mousePos);
	void onLeftClicDown(glm::vec2 mousePos);
	void onLeftClicUp();
	void onStandardKeyDown(char key);
	void onStandardKeyUp(char key);

private:
	glm::vec2 m_mousePos;
	bool m_leftClicIsDown;
	bool m_spaceBarIsDown;
	glm::vec2 m_mousePosWhenLeftClicDown;
};