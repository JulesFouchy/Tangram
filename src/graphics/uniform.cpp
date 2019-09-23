#include "uniform.hpp"

Uniform::Uniform(GLuint shaderID, const std::string name, uniformType value)
	: m_name     ( name ),
	  m_location ( glGetUniformLocation(shaderID, name.c_str()) ),
	  m_value    ( value )
{
}