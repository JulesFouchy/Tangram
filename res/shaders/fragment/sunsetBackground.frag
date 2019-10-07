#version 440 core

layout(location = 0) out vec4 fragColor;
in vec2 v_texCoord;

uniform float u_noiseScale; // default 0.03 min 0 max 2
uniform vec2 u_sunPos; // default 0 0
uniform float u_sunRadius; // default 2.976 min 0 max 10
uniform float u_Pow; // default 6.131 min 0 max 10
uniform vec3 u_SkyColor; // default (0.949f, 0.687f, 0.625f)
uniform vec3 u_SunColor; // default (1.000f, 0.869f, 0.291f)

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}
void main() {
	vec2 uv = v_texCoord;
	vec3 color;
	float t = smoothstep(1, 0,length(uv-u_sunPos)/u_sunRadius);
	color = mix(u_SkyColor, u_SunColor, t) + pow(t, u_Pow);
	fragColor = vec4(color,1.0) + u_noiseScale*rand(uv);
}