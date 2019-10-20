#version 440 core

layout(location = 0) out vec4 fragColor;
in vec2 v_texCoord;

uniform vec2 u_Begin; // POINT2D default 0 0
uniform float u_Scale; // default 0.149 min 0 max 5
uniform int u_NbIter; // default 17 min 1 max 25
uniform float u_Width; // default 0.089 min 0 max 3
uniform float u_Height; // default 1.071 min 0 max 3
uniform float u_smoothMinPow; // default 10 min 0 max 64
uniform float u_GlowRadius; // default 0.001 min 0 max 3
uniform float u_Angle; // default 0.449 min 0 max 3.14
uniform float u_ScaleAmount; // default 0.869 min 0 max 1
uniform vec3 u_GlowColor; // default 0.861f, 0.163f, 0.163f
uniform vec3 u_ColorTrunk; // default 0.816f, 0.618f, 0.109f
uniform vec3 u_ColorLeaves; // default 0.861f, 0.163f, 0.163f

float smoothMin( float a, float b)
{
    float res = exp2( -u_smoothMinPow*a ) + exp2( -u_smoothMinPow*b );
    return -log2( res )/u_smoothMinPow;
}

float TAU = 6.28;

vec2 fromAngle(float theta){
	return vec2(cos(theta), sin(theta));
}

vec2 rotate( vec2 v, float theta ){
	float c = cos(theta);
	float s = sin(theta);
	return vec2( c*v.x - s*v.y, s*v.x + c*v.y);
}

float rectangleSDF( vec2 uv ){
	return max(abs(uv.x) - u_Width, abs(uv.y) - u_Height);
}

float treeSDF( vec2 uv, out int iterMinReached ){
	float minDist = rectangleSDF(uv);
	iterMinReached = 0;

	for (int i = 0; i < u_NbIter; ++i){
		uv.y -= u_Height;
		if( uv.x < 0.)
			uv = rotate(uv, -u_Angle);
		else
			uv = rotate(uv, u_Angle);
		uv /= u_ScaleAmount;

		float newDist = rectangleSDF(uv);
		if( newDist < minDist ){
			iterMinReached = i;
		}
		minDist = smoothMin(newDist, minDist);
	}

	return minDist;
}

void main() {
	vec2 uv = (v_texCoord - u_Begin) / u_Scale;
	vec3 color;

	int iterMinReached;
	float d = treeSDF( uv, iterMinReached );

	color = mix(u_ColorTrunk, u_ColorLeaves, float(iterMinReached) / u_NbIter);
	float glow = smoothstep(u_GlowRadius, 0., d);
	float alpha = d < 0. ? 1. : glow ;
	if( glow < 0.99999 ){
		color += u_GlowColor;
	}
	fragColor = vec4(color,alpha);
}