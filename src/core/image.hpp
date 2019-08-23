#pragma once

#include "GL/glew.h"

#include <iostream>

#include "glm/glm.hpp"

#include "graphics/shader.hpp"

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

	void show(glm::mat4x4 transform, glm::mat4x4 projection);
	void show(glm::mat4x4 transform);

	void save(const std::string& filePath);

	static void initialize();

private:
	unsigned char* pixels;
	int width;
	int height;
	float aspectRatio;
	PixelFormat pixelFormat;
	int BPP;

	std::string filePath;
	unsigned int rendererId;

	unsigned int bytesPerPixel(PixelFormat format);
	GLint GLpixelInternalFormat(PixelFormat format);
	GLenum GLpixelFormat(PixelFormat format);

	//For rendering
	unsigned int m_fullQuadVBid;
	unsigned int m_fullQuadIBOid;
	static Shader standardShader;
};