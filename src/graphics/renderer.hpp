#pragma once

#include "GL/glew.h"
#include <SDL.h>
#include "glm/glm.hpp"

class Renderer {
public:
	Renderer();
	~Renderer();

	void drawFullQuad();

private:
	unsigned int m_fullQuadVBid;
	unsigned int m_fullQuadIBOid;
};