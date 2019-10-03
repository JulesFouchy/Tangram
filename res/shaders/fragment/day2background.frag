#version 440 core

layout(location = 0) out vec4 fragColor;

uniform float u_noiseScale; // default 0.03 min 0 max 2
uniform vec3 u_Color1; // default (0.075f, 0.359f, 0.636f)
uniform vec3 u_Color2; // default (1.000f, 0.870f, 0.296f)
uniform vec2 u_sunPos; // default -0.909 -2 min -5 -5 max 2 2 
uniform float u_sunRadius; // default 4.186 min 0 max 10

in vec2 v_texCoord;

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}
void main() {
	vec2 uv = v_texCoord;
	vec3 color = u_Color1;
	float t = smoothstep(1, 0,length(uv-u_sunPos)/u_sunRadius);
	color = mix(u_Color1, u_Color2, t*t);
	fragColor = vec4(color,1.0) + u_noiseScale*rand(uv);
}