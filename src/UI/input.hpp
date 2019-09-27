#pragma once

#include "glm/glm.hpp"

#include "helper/timestamp.hpp"
#include "SDL.h"
#include "keys.hpp"

class Input {

public:
	Input() = delete;
	static void Initialize();
	~Input();
	static void update();

	static glm::vec2 getMousePosition();
	static bool leftClicIsDown();
	static bool spaceBarIsDown();
	static bool keyIsDown(Key key);
	static glm::vec2 getMousePosWhenLastLeftClic();

	static void onLeftClicDown();
	static void onLeftClicUp();
	static void onDoubleLeftClic();
	static void onKeyDown(Key key);
	static void onKeyUp(Key key);
private:
	static bool m_leftClicIsDown;
	static bool m_spaceBarIsDown;
	static glm::vec2 m_mousePosWhenLastLeftClic;
	static Timestamp timeSinceLastLeftClic;
};