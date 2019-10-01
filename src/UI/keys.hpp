#pragma once

#include <variant>

enum SpecialKey {
	ALT,
	CTRL,
	SHIFT,
	SPACE,
	F5
};

typedef std::variant<SpecialKey, char> Key;