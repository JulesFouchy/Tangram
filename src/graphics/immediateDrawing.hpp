#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "shader.hpp"

class ImmediateDrawing {
public:
	ImmediateDrawing() = delete;
	static void initialize();
	~ImmediateDrawing();

	static void setViewProjMatrix(glm::mat4x4 viewProjMatrix);

	static void rect(float x, float y, float w, float h);

private:
	static unsigned int m_fullQuadVBid;
	static unsigned int m_fullQuadIBOid;
	static glm::mat4x4 m_viewProjMatrix;
	static glm::vec4 m_color;
	static Shader m_flatColorShader;
};