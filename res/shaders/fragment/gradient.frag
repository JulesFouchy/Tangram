#version 440 core

layout(location = 0) out vec4 fragColor;

uniform float u_angle; // default 0 | min 0 | max 6.2831853
uniform float u_scale; // default 0.5 min 0 max 2
uniform vec2 u_center; // default 0 0 | min -5 -5 | max 5 5
uniform vec3 u_Color1; // default 0 0 0
uniform vec3 u_Color2; // default 1 1 1

in vec2 v_texCoord;

void main() {
	vec2 uv = v_texCoord;
	vec2 center = u_center;
	float projV = dot(uv-center, vec2(-sin(u_angle), cos(u_angle)));
	float t = (clamp( projV / u_scale, -1. , 1.) + 1.) *0.5;
	vec3 color = mix(u_Color1, u_Color2, t);
	fragColor = vec4(color, 1.0);
}