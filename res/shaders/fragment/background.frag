#version 440 core

layout(location = 0) out vec4 color;

void main() {
	vec2 pos = vec2( gl_FragCoord.x / 1280.0 , gl_FragCoord.y / 720.0 );
	if(  )
	color = vec4(0.2, 0.9, 0.9, 1.0);
}