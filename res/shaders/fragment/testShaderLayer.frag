#version 440 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

void main() {
	color = vec4(v_texCoord, 0.0, 1.0);
}