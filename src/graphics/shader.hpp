#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader {
public:
	Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath, bool compileShader = true);
	~Shader();

	void bind();
	void compile();

	//Uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
	int getUniformLocation(const std::string& uniformName);
	void setUniformMat4f(const std::string& uniformName, glm::mat4& mat);
	void setUniform3f(const std::string& uniformName, glm::vec3 v);
	void setUniform4f(const std::string& uniformName, glm::vec4 v);
	void setUniform1f(const std::string& uniformName, float v);
	void setUniform1i(const std::string& uniformName, int v);

private:
	unsigned int m_shaderId;
	std::string m_vertexShaderFilepath;
	std::string m_fragmentShaderFilepath;
	/* Utilities to open files and compile shaders */
	std::string parseFile(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
};