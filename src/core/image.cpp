#include "image.hpp"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "utilities/display.hpp"

#include "spdlog/spdlog.h"
#include "glm/gtc/matrix_transform.hpp"

Shader Image::standardShader = Shader("res/shaders/vertex/texture.vert", "res/shaders/fragment/texture_standard.frag", false);

void Image::show(glm::mat4x4 transform, glm::mat4x4 projection) {
	//Bind texture
	glBindTexture(GL_TEXTURE_2D, rendererId);
	//Shader
	standardShader.bind();
	standardShader.setUniform1i("u_textureSlot", 0);

	//MVP
	/*glm::mat4x4 model = glm::mat4x4(1.0f);
	model = glm::translate(model, glm::vec3(center, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, glm::vec3(scale, scale, 1.0f));*/
	
	glm::mat4x4 mvp = projection * transform;
	standardShader.setUniformMat4f("u_mvp", mvp);

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

void Image::show(glm::mat4x4 transform) {
	show(transform, Display::getProjMat());
}

void Image::save(const std::string& filePath) {
	stbi_flip_vertically_on_write(1);
	stbi_write_jpg(filePath.c_str(), width, height, bytesPerPixel(pixelFormat), pixels, 100);
	//stbi_write_png(filePath.c_str(), width, height, bytesPerPixel(pixelFormat), pixels, 0);
}

Image::Image(const std::string& filePath)
	: pixels(nullptr), width(0), height(0), aspectRatio(1.0f), pixelFormat(RGBA), filePath(filePath), rendererId(0)
{
	//
	spdlog::info("[Opening image] " + filePath);
	//Load file
	stbi_set_flip_vertically_on_load(1);
	pixels = stbi_load(filePath.c_str(), &width, &height, &BPP, bytesPerPixel(pixelFormat));
	if (!pixels)
		spdlog::warn("Couldn't open " + filePath);
	aspectRatio = (float) width / height;
	//Gen texture
	glGenTextures(1, &rendererId);
	glBindTexture(GL_TEXTURE_2D,rendererId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GLpixelInternalFormat(pixelFormat), width, height, 0, GLpixelFormat(pixelFormat), GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Gen buffers
	float minX = Display::getMinY() * aspectRatio;
	float maxX = Display::getMaxY() * aspectRatio;
	float minY = Display::getMinY();
	float maxY = Display::getMaxY();

	float vertices[] = {
		//Position     TexCoord
		minX, minY,   0.0f, 0.0f,
		minX, maxY,   0.0f, 1.0f,
		maxX, maxY,   1.0f, 1.0f,
		maxX, minY,   1.0f, 0.0f,
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

void Image::initialize() {
	//Shaders must be compiled after openGl was initialized
	standardShader.compile();
}

Image::~Image() {
	spdlog::info("[Image Destructed] " + filePath);
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