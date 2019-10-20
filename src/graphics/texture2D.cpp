#include "texture2D.hpp"

#include "stb_image/stb_image_write.h"

#include "helper/display.hpp"
#include "helper/maths.hpp"

#include "UI/log.hpp"
#include "glm/gtc/matrix_transform.hpp"

Shader Texture2D::standardShader = Shader("res/shaders/vertex/texture.vert", "res/shaders/fragment/texture_standard.frag", false);
std::vector<bool> Texture2D::isSlotUsed;

Texture2D::Texture2D()
	: m_width(0), m_height(0), m_aspectRatio(1.0f), m_pixelFormat(RGBA), m_textureID(0), m_textureSlot(-1), m_rectVertexArray(), m_BPP(0)
{
	// Gen texture
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
	m_aspectRatio = (float)m_width / m_height;

	glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GLpixelInternalFormat(m_pixelFormat), m_width, m_height, 0, GLpixelFormat(m_pixelFormat), GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_rectVertexArray.Initialize(m_aspectRatio, ZERO_TO_ONE__ZERO_TO_ONE);
}

void Texture2D::ClassInitialization() {
	for (int i = 0; i < MAX_NB_TEXTURES; ++i)
		isSlotUsed.push_back(false);
	//Shaders must be compiled after openGl was initialized
	standardShader.compile();
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &m_textureID);
}

void Texture2D::show(glm::mat4x4 transform, glm::mat4x4 projection) {
	// Bind texture
	bind();
	// Shader
	standardShader.bind();
	standardShader.setUniform1i("u_textureSlot", getSlot());
	glm::mat4x4 mvp = projection * transform;
	standardShader.setUniformMat4f("u_mvp", mvp);
	// Vertex array
	m_rectVertexArray.binddrawunbind();
	// Unbind texture
	unbind();
}

void Texture2D::show(glm::mat4x4 transform) {
	show(transform, Display::getProjMat());
}

void Texture2D::bind() {
	// find slot to bind to
	if (m_textureSlot == -1) {
		auto it = std::find(isSlotUsed.cbegin(), isSlotUsed.cend(), false);
		if (it == isSlotUsed.cend())
			spdlog::error("[Texture2D::bind] no empty slot found !");
		else
			m_textureSlot = std::distance(isSlotUsed.cbegin(), it);
	}
	else {
		spdlog::error("[Texture2D::bind] not correctly unbound");
	}
	isSlotUsed[m_textureSlot] = true;
	//spdlog::warn("binding to slot {}", m_textureSlot);
	// select slot
	glActiveTexture(GL_TEXTURE0 + m_textureSlot);
	// bind
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture2D::unbind() {
	if (m_textureSlot > -1) {
		//spdlog::warn("unbinding slot {}", m_textureSlot);
		isSlotUsed[m_textureSlot] = false;
		m_textureSlot = -1;
		//
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		spdlog::warn("[Texture2D::unbind] trying to unbind a texture that wasn't bound");
	}
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
		spdlog::error("[Texture2D::bytesPerPixel] Unknown pixel format : {}", format);
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
		spdlog::error("[Texture2D::GLpixelInternalFormat] Unknown pixel format {}", format);
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
		spdlog::error("[Texture2D::GLpixelFormat] Unknown pixel format {}", format);
		break;
	}
}