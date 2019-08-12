#include "shader.hpp"

#include <GL/glew.h>

#include <fstream>
#include <iostream>

Shader::Shader() {}

Shader::Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath) {
	m_shaderId = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, parseFile(vertexShaderFilepath));
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, parseFile(fragmentShaderFilepath));

	glAttachShader(m_shaderId, vs);
	glAttachShader(m_shaderId, fs);
	glLinkProgram(m_shaderId);
	glValidateProgram(m_shaderId);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader() {
	glDeleteProgram(m_shaderId);
}

void Shader::bind() {
	glUseProgram(m_shaderId);
}

//Uniforms

int Shader::getUniformLocation(const std::string& uniformName) {
	if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[uniformName];
	}

	int location = glGetUniformLocation(m_shaderId, uniformName.c_str());
	//if (location == -1) {
	//	spdlog::warn("[Shader] uniform '{}' doesn't exist !", name);
	//}
	m_UniformLocationCache[uniformName] = location;
	return location;
}

void Shader::setUniformMat4f(const std::string& uniformName, glm::mat4& mat) {
	glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform3f(const std::string& uniformName, glm::vec3 v) {
	glUniform3f(getUniformLocation(uniformName), v.x, v.y, v.z);
}

void Shader::setUniform1f(const std::string& uniformName, float v) {
	glUniform1f(getUniformLocation(uniformName), v);
}

/* Utilities to open files and compile shaders */

std::string Shader::parseFile(const std::string& filepath) {
	std::ifstream stream(filepath);
	if (!stream) {
		std::cout << "Failed to open file " << filepath.c_str() << std::endl;
		return "";
	}

	std::string str = "";
	std::string tempLine = "";
	while (getline(stream, tempLine)) {
		str += tempLine + '\n';
	}
	stream.close();
	return str;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//Debug
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") << "shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}
