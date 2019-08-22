#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "shader.hpp"

class ImmediateDrawing {
public:
	ImmediateDrawing() = delete;
	static void initialize();
	static void genBuffers();
	~ImmediateDrawing();

	static void setViewProjMatrix(glm::mat4x4 viewProjMatrix);
	static void setColor(float r, float g, float b, float a);

	static void rect(float x, float y, float w, float h);
	static void rectOutline(float x, float y, float w, float h, float thickness);

private:
	static unsigned int m_fullQuadVBid;
	static unsigned int m_fullQuadIBOid;
	static glm::mat4x4 m_viewProjMatrix;
	static glm::vec4 m_color;
	static Shader m_flatColorShader;
};