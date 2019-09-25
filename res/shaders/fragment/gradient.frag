#version 440 core

layout(location = 0) out vec4 fragColor;

uniform float u_angle; // default 0 | min 0 | max 6.2831853
uniform vec2 u_center; // default 0 0 | min -5 -5 | max 5 5
uniform vec3 u_Color1; // default 0 0 0
uniform vec3 u_Color2; // default 1 1 1
//uniform int N;

in vec2 v_texCoord;

void main() {
	float projV = dot(v_texCoord-u_center, vec2(-sin(u_angle), cos(u_angle)));
	float t = projV * 0.5 + 0.5;
	vec3 color = (1.0-t) * u_Color1 + t * u_Color2;
	fragColor = vec4(color, 1.0);
}