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

class Texture2D {
friend class Layer;
friend class FrameBuffer;
public:
	static void ClassInitialization();

	void Initialize(int width, int height, int BPP, unsigned char* pixels = nullptr);
	virtual ~Texture2D();

	void show(glm::mat4x4 transform, glm::mat4x4 projection, float texCoordMinX = 0.0f, float texCoordMaxX = 1.0f, float texCoordMinY = 0.0f, float texCoordMaxY = 1.0f);
	void show(glm::mat4x4 transform, float texCoordMinX = 0.0f, float texCoordMaxX = 1.0f, float texCoordMinY = 0.0f, float texCoordMaxY = 1.0f);

	void bind();

	void save(const std::string& filePath);

	inline float getAspectRatio() { return m_aspectRatio; }

	inline unsigned int getID() { return m_textureID; }

protected:
	Texture2D();
protected:
	std::string m_debugName;
	unsigned char* m_pixels;
	int m_width;
	int m_height;
	float m_aspectRatio;
	PixelFormat m_pixelFormat;
	int m_BPP;

	unsigned int m_textureID;

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

public:
	static unsigned int bytesPerPixel(PixelFormat format);
	static GLint GLpixelInternalFormat(PixelFormat format);
	static GLenum GLpixelFormat(PixelFormat format);
};