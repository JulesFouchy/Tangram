#version 440 core

layout(location = 0) out vec4 fragColor;

uniform float u_pow; // default 2 min -0.15 max 4
uniform float u_scale; // default 2.44 min 0 max 10
uniform vec3 u_Color; // default 1.0 1.0 1.0

in vec2 v_texCoord;

void main() {
	vec2 uv = v_texCoord;
	uv *= sign(uv); // make both coord > 0 so that we are in the first quadrant
	if(uv.y>uv.x){ // put us in the bottom half of the first quadrant
		uv = uv.yx;
	}
	vec3 color;
	float x = uv.x*u_scale;
	if(uv.y * u_scale <= x*exp(-pow(x, u_pow))){
		color = u_Color;
	}
	else{
	  discard;
	}
	fragColor = vec4(color,1.0);
}