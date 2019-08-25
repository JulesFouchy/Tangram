#include "image.hpp"

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "utilities/display.hpp"
#include "utilities/maths.hpp"

#include "spdlog/spdlog.h"
#include "glm/gtc/matrix_transform.hpp"

Shader Image::standardShader = Shader("res/shaders/vertex/texture.vert", "res/shaders/fragment/texture_standard.frag", false);

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
	aspectRatio = (float)width / height;
	//Gen texture
	glGenTextures(1, &rendererId);
	glBindTexture(GL_TEXTURE_2D, rendererId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GLpixelInternalFormat(pixelFormat), width, height, 0, GLpixelFormat(pixelFormat), GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Vertex Buffer
	glGenBuffers(1, &m_quadVBid);
	computeAndSendVertexBuffer();
	//Index buffer
	glGenBuffers(1, &m_quadIBOid);
	unsigned int indices[] = {
		0, 2, 1,
		0, 3, 2
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Image::computeAndSendVertexBuffer(float texCoordMinX, float texCoordMaxX, float texCoordMinY, float texCoordMaxY) {
	float minX = Display::getMinY() * aspectRatio;
	float maxX = Display::getMaxY() * aspectRatio;
	float minY = Display::getMinY();
	float maxY = Display::getMaxY();

	float actualMinX = Maths::map(texCoordMinX, 0.0f, 1.0f, minX, maxX);
	float actualMaxX = Maths::map(texCoordMaxX, 0.0f, 1.0f, minX, maxX);
	float actualMinY = Maths::map(texCoordMinY, 0.0f, 1.0f, minY, maxY);
	float actualMaxY = Maths::map(texCoordMaxY, 0.0f, 1.0f, minY, maxY);

	float vertices[] = {
		      //Position                   TexCoord
		actualMinX, actualMinY,   texCoordMinX, texCoordMinY,
		actualMinX, actualMaxY,   texCoordMinX, texCoordMaxY,
		actualMaxX, actualMaxY,   texCoordMaxX, texCoordMaxY,
		actualMaxX, actualMinY,   texCoordMaxX, texCoordMinY,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBid);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Image::initialize() {
	//Shaders must be compiled after openGl was initialized
	standardShader.compile();
}

Image::~Image() {
	spdlog::info("[Image Destructed] " + filePath);
	glDeleteTextures(1, &rendererId);
	if (pixels)
		stbi_image_free(pixels);
}

void Image::show(glm::mat4x4 transform, glm::mat4x4 projection, float texCoordMinX, float texCoordMaxX, float texCoordMinY, float texCoordMaxY) {
	//Recompute vertex buffer
	computeAndSendVertexBuffer(texCoordMinX, texCoordMaxX, texCoordMinY, texCoordMaxY);
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
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBid);
		//pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		//texCoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*) (2 * sizeof(float)) );
	//IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBOid);
	//Blend mode
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Draw call
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Image::show(glm::mat4x4 transform, float texCoordMinX, float texCoordMaxX, float texCoordMinY, float texCoordMaxY) {
	show(transform, Display::getProjMat(), texCoordMinX, texCoordMaxX, texCoordMinY, texCoordMaxY);
}

void Image::save(const std::string& filePath) {
	stbi_flip_vertically_on_write(1);
	stbi_write_jpg(filePath.c_str(), width, height, bytesPerPixel(pixelFormat), pixels, 100);
	//stbi_write_png(filePath.c_str(), width, height, bytesPerPixel(pixelFormat), pixels, 0);
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