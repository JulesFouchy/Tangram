#pragma once

#include "glm/glm.hpp"

#include "utilities/timestamp.hpp"

class Input {

public:
	Input() = delete;
	static void initialize();
	~Input();
	static void update();

	static glm::vec2 getMousePosition();
	static bool leftClicIsDown();
	static bool spaceBarIsDown();
	static glm::vec2 getMousePosWhenLastLeftClic();

	static void onLeftClicDown();
	static void onLeftClicUp();
	static void onStandardKeyDown(char key);
	static void onStandardKeyUp(char key);
private:
	static void onDoubleLeftClic();

public:
	static bool bDoubleLeftClic;
private:
	static bool m_leftClicIsDown;
	static bool m_spaceBarIsDown;
	static glm::vec2 m_mousePosWhenLastLeftClic;
	static Timestamp timeSinceLastLeftClic;
};