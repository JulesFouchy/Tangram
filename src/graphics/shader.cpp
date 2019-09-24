#include "shader.hpp"

#include <GL/glew.h>

#include <fstream>
#include <iostream>

#include "UI/log.hpp"

Shader::Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath, bool compileShader)
	: m_shaderId(-1), m_vertexShaderFilepath(vertexShaderFilepath), m_fragmentShaderFilepath(fragmentShaderFilepath)
{
	if (compileShader) {
		compile();
	}
}

Shader::~Shader() {
	glDeleteProgram(m_shaderId);
}

void Shader::bind() {
	glUseProgram(m_shaderId);
}

void Shader::compile() {
	spdlog::info("[Compiling Shader] " + m_vertexShaderFilepath + " & " + m_fragmentShaderFilepath);
	m_shaderId = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, parseFile(m_vertexShaderFilepath));
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, parseFile(m_fragmentShaderFilepath));

	glAttachShader(m_shaderId, vs);
	glAttachShader(m_shaderId, fs);
	glLinkProgram(m_shaderId);
	glValidateProgram(m_shaderId);

	glDeleteShader(vs);
	glDeleteShader(fs);
	Log::separationLine();
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

void Shader::setUniform1i(const std::string& uniformName, int v) {
	glUniform1i(getUniformLocation(uniformName), v);
}
void Shader::setUniform1f(const std::string& uniformName, float v) {
	glUniform1f(getUniformLocation(uniformName), v);
}
void Shader::setUniform2f(const std::string& uniformName, glm::vec2 v) {
	glUniform2f(getUniformLocation(uniformName), v.x, v.y);
}
void Shader::setUniform3f(const std::string& uniformName, glm::vec3 v) {
	glUniform3f(getUniformLocation(uniformName), v.x, v.y, v.z);
}
void Shader::setUniform4f(const std::string& uniformName, glm::vec4 v) {
	glUniform4f(getUniformLocation(uniformName), v.x, v.y, v.z, v.w);
}
void Shader::setUniformMat4f(const std::string& uniformName, glm::mat4& mat) {
	glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setUniform(const Uniform& uniform) {
	/*if (auto myInt = std::get_if<int>(&uniform.getValue())) {
		glUniform1i(uniform.getLocation(), *myInt);
	}
	else if (auto myFloat = std::get_if<float>(&uniform.getValue())) {
		glUniform1f(uniform.getLocation(), *myFloat);
	}
	else if (auto myVec2 = std::get_if<glm::vec2>(&uniform.getValue())) {
		glUniform2f(uniform.getLocation(), myVec2->x, myVec2->y);
	}
	else if (auto myVec3 = std::get_if<glm::vec3>(&uniform.getValue())) {
		glUniform3f(uniform.getLocation(), myVec3->x, myVec3->y, myVec3->z);
	}
	else if (auto myVec4 = std::get_if<glm::vec4>(&uniform.getValue())) {
		glUniform4f(uniform.getLocation(), myVec4->x, myVec4->y, myVec4->z, myVec4->w);
	}*/
	glUniform1f(uniform.getLocation(), uniform.getValue());
}

/* Utilities to open files and compile shaders */

std::string Shader::parseFile(const std::string& filepath) {
	std::ifstream stream(filepath);
	if (!stream) {
		spdlog::warn("Failed to open file " + filepath);
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
		spdlog::warn("Failed to compile {} {}", (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") , "shader");
		spdlog::warn(message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}
