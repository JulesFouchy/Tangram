#pragma once

#include <variant>

enum SpecialKey {
	ALT,
	CTRL,
	SHIFT,
	SPACE
};

typedef std::variant<SpecialKey, char> Key;