#pragma once

#include "GL/glew.h"

#include <iostream>

enum PixelFormat {
	RGBA,
	RGB,
	GA,
	A
};

class Image {
public :
	Image(const std::string& filePath);
	~Image();

	void show(float x, float y);

private:
	unsigned char* pixels;
	int width;
	int height;
	PixelFormat pixelFormat;
	int BPP;

	std::string filePath;
	unsigned int rendererId;

	unsigned int bytesPerPixel(PixelFormat format);
	GLint GLpixelInternalFormat(PixelFormat format);
	GLenum GLpixelFormat(PixelFormat format);
};