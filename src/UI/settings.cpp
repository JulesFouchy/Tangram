#include "settings.hpp"

float Settings::ALT_ORIGIN_RADIUS = 0.012f;
float Settings::START_ROTATING_MIN_RADIUS = 0.116f;
float Settings::START_ROTATING_MAX_RADIUS = 0.138f;

int Settings::NB_ANGLES_POSSIBLE_FOR_ROTATION = 16;
float Settings::ZOOM_FACTOR = 0.8f;

unsigned char Settings::MIN_ALPHA_TO_GRAB_LAYER = 10;

bool Settings::SAVE_ALT_ORIGIN_TRANSLATION_IN_HISTORY = false;

std::string Settings::m_EXECUTABLE_FILEPATH = "C:/Dev/Tangram/";