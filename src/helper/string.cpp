#include "string.hpp"

size_t String::beginningOfNextWord(const std::string& myString, size_t beginPos) {
	return myString.find_first_not_of(" ,;/()|", beginPos);
}

size_t String::endOfNextWord(const std::string& myString, size_t beginPos) {
	size_t pos = myString.find_first_of(" ,;/()|", beginPos);
	return pos == std::string::npos ? myString.size() : pos;
}

std::string String::getNextWord(const std::string& myString, size_t* currentPosition) {
	size_t beginWord = beginningOfNextWord(myString, *currentPosition);
	size_t endWord = endOfNextWord(myString, beginWord);
	*currentPosition = endWord;
	if (beginWord != std::string::npos) {
		return myString.substr(beginWord, endWord - beginWord);
	}
	else
		return "";
}

void String::replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}