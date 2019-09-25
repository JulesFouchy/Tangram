#pragma once

#include <string>

namespace ParseShader {
	size_t beginningOfNextWord(const std::string& myString, size_t beginPos);
	size_t endOfNextWord(const std::string& myString, size_t beginPos);
	std::string getNextWord(const std::string& myString, size_t* currentPosition);
}