#include "image.hpp"

#include "stb_image/stb_image.h"

void Image::show(float x, float y) {
	glBindTexture(GL_TEXTURE_2D, rendererId);
}

Image::Image(const std::string& filePath)
	: pixels(nullptr), width(0), height(0), pixelFormat(RGBA), filePath(filePath), rendererId(0)
{
	stbi_set_flip_vertically_on_load(1);
	pixels = stbi_load(filePath.c_str(), &width, &height, &BPP, bytesPerPixel(pixelFormat));

	glGenTextures(1, &rendererId);
	glBindTexture(GL_TEXTURE_2D,rendererId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GLpixelInternalFormat(pixelFormat), width, height, 0, GLpixelFormat(pixelFormat), GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
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
		break;
	}
}