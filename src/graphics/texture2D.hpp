#pragma once

#include "GL/glew.h"
#include "rectVAO.hpp"

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
	~Texture2D();

	void show(glm::mat4x4 transform, glm::mat4x4 projection);
	void show(glm::mat4x4 transform);

	void bind();

	void save(const std::string& filePath);

	inline float getAspectRatio() { return m_aspectRatio; }
	inline int getWidth() { return m_width; }
	inline int getHeight() { return m_height; }
	inline int getBPP() { return m_BPP; }
	inline unsigned char* getPixels() { return m_pixels; }
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
	RectVAO m_rectVertexArray;
	static Shader standardShader;

public:
	static unsigned int bytesPerPixel(PixelFormat format);
	static GLint GLpixelInternalFormat(PixelFormat format);
	static GLenum GLpixelFormat(PixelFormat format);
};