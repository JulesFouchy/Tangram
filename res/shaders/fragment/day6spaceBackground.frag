#version 440 core

layout(location = 0) out vec4 fragColor;
in vec2 v_texCoord;

uniform vec2 u_Center; // default 0 0 min -2 -2 max 4 4 
uniform float u_Radius; // default 0.798 min 0 max 4
uniform float u_Thickness; // default 0.119 min 0 max 2
uniform float u_ThicknessPow; // default 1 min 0 max 5
uniform float u_Amplitude; // default 0.786 min 0 max 2
uniform float u_HoleProba; // default 1 min 0 max 1
uniform float u_RandScaleX; // default 1.25 min 0 max 5
uniform float u_RandScaleY; // default 0.327 min 0 max 5

uniform float u_noiseScale; // default 0.06 min 0 max 2
uniform vec3 u_BackColor; // default 0 0 0
uniform vec3 u_PointColor; // default 1 1 1

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}
void main() {
	vec2 uv = v_texCoord - u_Center;
	uv.x /= (uv.y);
	uv.x += rand(uv)*u_RandScaleX;
	uv.y += rand(uv)*u_RandScaleY;
	vec3 color = u_BackColor;

	color += vec3(u_noiseScale*rand(uv));
	if( rand(uv + vec2(15, 0.3333)) < u_HoleProba )
		color += u_PointColor * u_Amplitude * ( pow(smoothstep(u_Radius+u_Thickness, u_Radius,length(uv)) - smoothstep(u_Radius, u_Radius - u_Thickness,length(uv)), u_ThicknessPow) );
	fragColor = vec4(color,1.0);
}