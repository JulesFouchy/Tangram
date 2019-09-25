#include "parseShader.hpp"

#include <algorithm>

size_t ParseShader::beginningOfNextWord(const std::string& myString, size_t beginPos) {
	return std::max( std::max( std::max(
			myString.find_first_not_of(' ', beginPos), myString.find_first_not_of(',', beginPos)), myString.find_first_not_of(';', beginPos)), myString.find_first_not_of('/', beginPos));
}

size_t ParseShader::endOfNextWord(const std::string& myString, size_t beginPos) {
	size_t pos = std::min( std::min( std::min(
				  myString.find(' ', beginPos), myString.find(',', beginPos)), myString.find(';', beginPos)), myString.find('/', beginPos));
	return pos == std::string::npos ? myString.size() : pos;
}