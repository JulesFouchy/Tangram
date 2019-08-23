#pragma once

#include "glm/glm.hpp"

class Input {

public:
	Input() = delete;
	static void initialize();
	~Input();

	static glm::vec2 getMousePosition();
	static bool leftClicIsDown();
	static bool spaceBarIsDown();
	static glm::vec2 getMousePosWhenLeftClicAndSpaceBarDown();

	static void onMouseMove(glm::vec2 mousePos);
	static void onLeftClicDown(glm::vec2 mousePos);
	static void onLeftClicUp();
	static void onStandardKeyDown(char key);
	static void onStandardKeyUp(char key);

private:
	static glm::vec2 m_mousePos;
	static bool m_leftClicIsDown;
	static bool m_spaceBarIsDown;
	static glm::vec2 m_mousePosWhenLeftClicAndSpaceBarDown;
};