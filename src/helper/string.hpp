#pragma once

#include <string>

namespace String {
	size_t beginningOfNextWord(const std::string& myString, size_t beginPos);
	size_t endOfNextWord(const std::string& myString, size_t beginPos);
	std::string getNextWord(const std::string& myString, size_t* currentPosition);
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
	std::string getFileExtension(const std::string& myString);
	std::string RemoveFolderHierarchy(const std::string& myString);
}