#version 440 core

layout(location = 0) out vec4 color;

void main() {
	vec2 pos = vec2( gl_FragCoord.x / 720.0 , gl_FragCoord.y / 720.0 );
	if( length(pos-vec2(0.5*1.6/0.9,0.5)) < 0.3 ){
		color = vec4(0.2, 0.9, 0.9, 1.0);
	}
	else{
		color = vec4(0.0,0.0,0.0,1.0);
	}
}