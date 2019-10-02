#version 440 core

layout(location = 0) out vec4 fragColor;

uniform float u_radiusInner; // default 0.5 min 0 max 3
uniform float u_radiusOuter; // default 0.8 min 0 max 3
uniform vec3 u_Color; // default 1.0 1.0 1.0

in vec2 v_texCoord;

void main() {
	vec2 uv = v_texCoord;
	vec3 color;

	float alpha = smoothstep(u_radiusOuter, u_radiusInner, length(uv));
	fragColor = vec4(u_Color,alpha);
}