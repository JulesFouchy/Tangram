#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader {
public:
	Shader(); //To create it unitialized in window (because we can't do it in an initializer list beacuse we initialize glew/glfw in the body of the window constructor and creating shader must be done after this
	Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath);
	~Shader();

	void bind();

	//Uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
	int getUniformLocation(const std::string& uniformName);
	void setUniformMat4f(const std::string& uniformName, glm::mat4& mat);
	void setUniform3f(const std::string& uniformName, glm::vec3 v);
	void setUniform1f(const std::string& uniformName, float v);
	void setUniform1i(const std::string& uniformName, int v);

private:
	unsigned int m_shaderId;

	/* Utilities to open files and compile shaders */
	std::string parseFile(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
};