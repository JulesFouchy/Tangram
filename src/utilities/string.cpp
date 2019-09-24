#include "string.hpp"

const std::string& String::getFirstWord(const std::string& myString, const size_t startPos) {
	return myString.substr(startPos, myString.find(' ', startPos));
}