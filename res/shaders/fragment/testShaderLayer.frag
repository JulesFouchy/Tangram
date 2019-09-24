#version 440 core

layout(location = 0) out vec4 fragColor;

uniform float u_Radius;

in vec2 v_texCoord;

void main() {
	float d = length(v_texCoord);
	vec3 color = vec3(1.0 - smoothstep(d-u_Radius,-0.02,0.02));
	fragColor = vec4(color, 1.0);
}