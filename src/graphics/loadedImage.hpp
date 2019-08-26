#pragma once

#include "texture2D.hpp"

class LoadedImage : public Texture2D {
public:
	LoadedImage(const std::string& filePath);
	~LoadedImage();
private:
	std::string m_filePath;
};