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

	void show(glm::mat4x4 transform, glm::mat4x4 projection, float texCoordMinX = 0.0f, float texCoordMaxX = 1.0f, float texCoordMinY = 0.0f, float texCoordMaxY = 1.0f);
	void show(glm::mat4x4 transform, float texCoordMinX = 0.0f, float texCoordMaxX = 1.0f, float texCoordMinY = 0.0f, float texCoordMaxY = 1.0f);

	void save(const std::string& filePath);

	inline float getAspectRatio() { return aspectRatio ;}

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
	unsigned int m_quadVBid;
	unsigned int m_quadIBOid;
	static Shader standardShader;
	//
	float lastTexCoordMinX;
	float lastTexCoordMaxX;
	float lastTexCoordMinY;
	float lastTexCoordMaxY;
private:
	void computeAndSendVertexBuffer(float texCoordMinX = 0.0f, float texCoordMaxX = 1.0f, float texCoordMinY = 0.0f, float texCoordMaxY = 1.0f);
};