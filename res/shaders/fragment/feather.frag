#version 440 core

layout(location = 0) out vec4 fragColor;

uniform float u_radius; // default 0.5 min 0 max 3
uniform float u_margin; // default 0.107 min 0 max 3
uniform float u_Xoffset; // default 0.56 min 0 max 1
uniform float u_Scale; // default 1.512 min 0 max 2
//uniform vec2 u_center; // POINT2D default 0 0 
uniform vec3 u_Color; // default 1.0 1.0 1.0

in vec2 v_texCoord;

float feather( vec2 uv ){
	return smoothstep(u_radius+u_margin/2., u_radius-u_margin/2., length(uv));
}

void main() {
	vec2 uv = v_texCoord / u_Scale;
	if (uv.x < 0 ){
		uv -= vec2( u_Xoffset / u_Scale , 0.);
		uv.x *= -1.;
	}
	else{
		uv += vec2( u_Xoffset / u_Scale , 0.);
	}
	float alpha = feather(uv);
	fragColor = vec4(u_Color,alpha);
}