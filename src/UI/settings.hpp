#pragma once

#include <string>

class Settings {
public:
	static float ALT_ORIGIN_RADIUS;
	static float START_ROTATING_MIN_RADIUS;
	static float START_ROTATING_MAX_RADIUS;

	static int NB_ANGLES_POSSIBLE_FOR_ROTATION;
	static float ZOOM_FACTOR;

	static unsigned char MIN_ALPHA_TO_GRAB_LAYER;

	static bool SAVE_ALT_ORIGIN_TRANSLATION_IN_HISTORY;
	inline static const std::string& EXECUTABLE_FILEPATH() { return m_EXECUTABLE_FILEPATH; }

private:
	static std::string m_EXECUTABLE_FILEPATH;
};