#pragma once

enum CoordinateSystem {
	ZERO_TO_ONE__ZERO_TO_ONE,
	MINUS_RATIO_TO_RATIO__MINUS_ONE_TO_ONE
};

class RectVAO {
public:
	RectVAO(float aspectRatio, CoordinateSystem coordSystem);
	~RectVAO();

	static void DrawCall();
	void bind();
	void unbind();
private:
	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;
	GLuint m_vertexArray;
};