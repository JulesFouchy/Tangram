#version 440 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform vec4 u_color;
uniform float u_innerRadius;

void main() {
	vec2 pos = v_texCoord;
	float d = length( pos - vec2(0.5) );
	float mrg = 0.01;
	float t = smoothstep( u_innerRadius/2.0-mrg, u_innerRadius/2.0+mrg, d) - smoothstep( 0.5-mrg, 0.5+mrg, d);
	gl_FragColor = u_color;
	gl_FragColor.a *= t;
}