#pragma once

#include <string>
#include <variant>

#include "GL/glew.h"

#include "openGLType.hpp"

#include "glm/glm.hpp"

#include "transforms/draggablePoint.hpp"

typedef std::variant<int, float, glm::vec2, DraggablePoint, glm::vec3, glm::vec4> UniformType;

class Uniform {
public:
	Uniform(GLuint shaderID, const std::string name, UniformType value, UniformType minValue, UniformType maxValue, bool itsAColor = false);
	~Uniform() = default;

	void set();

	inline const std::string& getName() const { return m_name; }
	inline int getLocation() const { return m_location; }
	const UniformType& getValue() const { return m_value; }
	UniformType* getValuePointer() { return &m_value; }
	bool GuiDragValue();

	void showDraggablePoints();
	
public:
	// Helpers
	static UniformType zero(OpenGLType type);
	static UniformType  one(OpenGLType type);
private:
	std::string m_name;
	int m_location;
	UniformType m_value;
	UniformType m_minValue;
	UniformType m_maxValue;

	bool m_bImAColor; // temporary !

	DraggablePoint m_pos2D_WS; // temporary ! only for vec2 uniforms
};