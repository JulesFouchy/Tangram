#pragma once

#include "GL/glew.h"

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

	void show(glm::vec2 center = glm::vec2(0.0f), float scale = 1.0f, float rotation = 0.0f);

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
	static glm::mat4x4 proj;
};