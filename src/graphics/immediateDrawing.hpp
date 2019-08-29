#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "shader.hpp"

class ImmediateDrawing {
public:
	ImmediateDrawing() = delete;
	static void Initialize();
	static void genBuffers();
	~ImmediateDrawing();

	static void setViewProjMatrix(glm::mat4x4 viewProjMatrix);
	static void setColor(float r, float g, float b, float a);
	inline static void setColor(glm::vec4 color) { m_color = color; }

	static void shaderlessFullscreenRect();

	static void rect(float x, float y, float w, float h);
	static void rectOutline(float x, float y, float w, float h, float thickness);

	static void windowMinusARectangle(glm::vec2 centerInFragSpace, float w, float h, float angle);

	static void ring(float x, float y, float innerR, float outerR);

private:
	static unsigned int m_fullQuadVBid;
	static unsigned int m_fullQuadIBOid;
	static glm::mat4x4 m_viewProjMatrix;
	static glm::vec4 m_color;
	static Shader m_flatColorShader;
	static Shader m_ringShader;
	static Shader m_windowMinusARectangleShader;
};