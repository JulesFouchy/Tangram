#version 440 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main() {
	color = vec4(0.0,0.0,0.0,0.0);//u_color;
}