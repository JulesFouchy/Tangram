#version 440 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform vec4 u_color;

uniform vec2 u_centerInFragSpace;
uniform vec2 u_wAxis;
uniform float u_width;
uniform vec2 u_hAxis;
uniform float u_height;

void main() {
	vec2 pos = gl_FragCoord.xy - u_centerInFragSpace;
	float dW = abs(dot(pos, u_wAxis));
	float dH = abs(dot(pos, u_hAxis));
	if( dW < 1.0 && dH < 1.0){
		discard;
	}
	gl_FragColor = u_color;
}