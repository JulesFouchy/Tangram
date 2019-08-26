#include "texture2D.hpp"

#include "stb_image/stb_image_write.h"

#include "utilities/display.hpp"
#include "utilities/maths.hpp"

#include "spdlog/spdlog.h"
#include "glm/gtc/matrix_transform.hpp"

Shader Texture2D::standardShader = Shader("res/shaders/vertex/texture.vert", "res/shaders/fragment/texture_standard.frag", false);

Texture2D::Texture2D()
	: m_debugName("texture2D"), m_pixels(nullptr), m_width(0), m_height(0), m_aspectRatio(1.0f), m_pixelFormat(RGBA), m_textureID(0),
	lastTexCoordMinX(0.0f), lastTexCoordMaxX(1.0f), lastTexCoordMinY(0.0f), lastTexCoordMaxY(1.0f), m_BPP(0), m_quadVBid(0), m_quadIBOid(0)
{
	//Gen texture
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Initialize(int width, int height, int BPP, unsigned char* pixels){
	m_BPP = BPP;
	m_width = width;
	m_height = height;
	m_pixels = pixels;
	m_aspectRatio = (float)m_width / m_height;

	glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GLpixelInternalFormat(m_pixelFormat), m_width, m_height, 0, GLpixelFormat(m_pixelFormat), GL_UNSIGNED_BYTE, m_pixels);
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

void Texture2D::computeAndSendVertexBuffer(float texCoordMinX, float texCoordMaxX, float texCoordMinY, float texCoordMaxY) {
	float minX = Display::getMinY() * m_aspectRatio;
	float maxX = Display::getMaxY() * m_aspectRatio;
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
	//
	lastTexCoordMinX = texCoordMinX;
	lastTexCoordMaxX = texCoordMaxX;
	lastTexCoordMinY = texCoordMinY;
	lastTexCoordMaxY = texCoordMaxY;
}

void Texture2D::Initialize() {
	//Shaders must be compiled after openGl was initialized
	standardShader.compile();
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &m_textureID);
	if (m_pixels)
		spdlog::warn("pixels of {} not freed !", m_debugName);
}

void Texture2D::show(glm::mat4x4 transform, glm::mat4x4 projection, float texCoordMinX, float texCoordMaxX, float texCoordMinY, float texCoordMaxY) {
	//Recompute vertex buffer
	if (0.0001f < abs(texCoordMinX - lastTexCoordMinX) + abs(texCoordMaxX - lastTexCoordMaxX) + abs(texCoordMinY - lastTexCoordMinY) + abs(texCoordMaxY - lastTexCoordMaxY)) {
		computeAndSendVertexBuffer(texCoordMinX, texCoordMaxX, texCoordMinY, texCoordMaxY);
		spdlog::info("recomputed vertex buffer of {}", m_debugName);
	}
	//Bind texture
	bind();
	//Shader
	standardShader.bind();
	standardShader.setUniform1i("u_textureSlot", 0);

	glm::mat4x4 mvp = projection * transform;
	standardShader.setUniformMat4f("u_mvp", mvp);

	//Draw quad
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBid);
	//pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	//texCoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));
	//IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBOid);
	//Blend mode
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Draw call
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Texture2D::show(glm::mat4x4 transform, float texCoordMinX, float texCoordMaxX, float texCoordMinY, float texCoordMaxY) {
	show(transform, Display::getProjMat(), texCoordMinX, texCoordMaxX, texCoordMinY, texCoordMaxY);
}

void Texture2D::bind() {
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture2D::save(const std::string& filePath) {
	stbi_flip_vertically_on_write(1);
	stbi_write_jpg(filePath.c_str(), m_width, m_height, bytesPerPixel(m_pixelFormat), m_pixels, 100);
	//stbi_write_png(filePath.c_str(), m_width, m_height, bytesPerPixel(m_pixelFormat), m_pixels, 0);
}

unsigned int Texture2D::bytesPerPixel(PixelFormat format) {
	switch (format) {
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
		spdlog::warn("Unknown pixel format : {}", format);
		break;
	}
}

GLint Texture2D::GLpixelInternalFormat(PixelFormat format) {
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
		spdlog::warn("Unknown pixel format {}", format);
		break;
	}
}

GLenum Texture2D::GLpixelFormat(PixelFormat format) {
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
		spdlog::warn("Unknown pixel format {}", format);
		break;
	}
}