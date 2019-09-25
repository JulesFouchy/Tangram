#include "string.hpp"

size_t ParseShader::beginningOfNextWord(const std::string& myString, size_t beginPos) {
	return myString.find_first_not_of(" ,;/()|", beginPos);
}

size_t ParseShader::endOfNextWord(const std::string& myString, size_t beginPos) {
	size_t pos = myString.find_first_of(" ,;/()|", beginPos);
	return pos == std::string::npos ? myString.size() : pos;
}

std::string ParseShader::getNextWord(const std::string& myString, size_t* currentPosition) {
	size_t beginWord = beginningOfNextWord(myString, *currentPosition);
	size_t endWord = endOfNextWord(myString, beginWord);
	*currentPosition = endWord;
	if (beginWord != std::string::npos) {
		return myString.substr(beginWord, endWord - beginWord);
	}
	else
		return "";
}