#include "image.hpp"

#include "stb_image/stb_image.h"

#include "spdlog/spdlog.h"

void Image::show(float x, float y) {
	//Bind texture
	glBindTexture(GL_TEXTURE_2D, rendererId);
	//Shader
	/* TODO
		have the shader as a static member of Image and bind it here
	*/
	//standardShader.bind();
	//standardShader.setUniform1i("u_textureSlot", 0);
	//Draw quad
	glBindBuffer(GL_ARRAY_BUFFER, m_fullQuadVBid);
		//pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		//texCoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*) (2 * sizeof(float)) );
	//IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullQuadIBOid);
	//Blend mode
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Draw call
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

Image::Image(const std::string& filePath)
	: pixels(nullptr), width(0), height(0), pixelFormat(RGBA), filePath(filePath), rendererId(0)
{
	//Load file
	stbi_set_flip_vertically_on_load(1);
	pixels = stbi_load(filePath.c_str(), &width, &height, &BPP, bytesPerPixel(pixelFormat));

	//Gen texture
	glGenTextures(1, &rendererId);
	glBindTexture(GL_TEXTURE_2D,rendererId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GLpixelInternalFormat(pixelFormat), width, height, 0, GLpixelFormat(pixelFormat), GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Gen buffers to render a quad on the whole screen
	float vertices[] = {
		//Position      TexCoord
		-1.0f, -1.0f,   0.0f, 0.0f,
		-1.0f,  1.0f,   0.0f, 1.0f,
	 	 1.0f,  1.0f,   1.0f, 1.0f,
	 	 1.0f, -1.0f,   1.0f, 0.0f,
	};
	glGenBuffers(1, &m_fullQuadVBid);
	glBindBuffer(GL_ARRAY_BUFFER, m_fullQuadVBid);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Gen index buffer
	unsigned int indices[] = {
		0, 2, 1,
		0, 3, 2
	};
	glGenBuffers(1, &m_fullQuadIBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullQuadIBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Image::~Image() {
	glDeleteTextures(1, &rendererId);
	if( pixels )
		stbi_image_free(pixels);
}

unsigned int Image::bytesPerPixel(PixelFormat format) {
	switch (format){
		case RGBA:
			return 4;
			break;
		case RGB:
			return 3;
			break;
		case GA:
			return 2;
			break;
		case A:
			return 1;
			break;
		default:
			spdlog::warn("Unknown pixel format for {} !", filePath);
			break;
	}
}

GLint Image::GLpixelInternalFormat(PixelFormat format) {
	switch (format) {
		case RGBA:
			return GL_RGBA8;
			break;
		case RGB:
			return GL_RGB8;
			break;
		case GA:
			return GL_RG8;
			break;
		case A:
			return GL_R8;
			break;
		default:
			spdlog::warn("Unknown pixel format for {} !", filePath);
			break;
	}
}

GLenum Image::GLpixelFormat(PixelFormat format) {
	switch (format) {
	case RGBA:
		return GL_RGBA;
		break;
	case RGB:
		return GL_RGB;
		break;
	case GA:
		return GL_RG;
		break;
	case A:
		return GL_R;
		break;
	default:
		spdlog::warn("Unknown pixel format for {} !", filePath);
		break;
	}
}