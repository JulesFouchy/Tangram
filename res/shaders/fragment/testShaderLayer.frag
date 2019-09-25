#version 440 core

layout(location = 0) out vec4 fragColor;

  uniform   float           u_Radius;//; // default 0.55 min -0.15 max 1.5
     uniform     vec3    u_Color  ;   //  default 1.0   1.0, 1.0  ;   
//uniform int N;

in vec2 v_texCoord;

void main() {
	float d = length(v_texCoord);
	vec4 color = (1.0 - smoothstep(d-u_Radius,-0.02,0.02)) * vec4(u_Color,1.0);
	//vec3 color = vec3(sin(N * v_texCoord.y * 3.14));
	fragColor = vec4(color);
}