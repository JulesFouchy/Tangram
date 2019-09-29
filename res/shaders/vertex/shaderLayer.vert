#version 440 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4x4 u_mvp;

out vec2 v_texCoord;

void main() {
	gl_Position = u_mvp * position;
	v_texCoord = texCoord;
}