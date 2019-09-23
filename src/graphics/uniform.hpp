#pragma once

#include <string>
#include <variant>

#include "GL/glew.h"

#include "glm/glm.hpp"

typedef std::variant<int, float, glm::vec2, glm::vec3, glm::vec4> uniformType;

class Uniform {
public:
	Uniform(GLuint shaderID, const std::string name, uniformType value);
	~Uniform() = default;

	inline int getLocation() const { return m_location; }
	inline uniformType getValue() const { return m_value; }
private:
	std::string m_name;
	int m_location;
	uniformType m_value;
};