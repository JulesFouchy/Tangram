#version 440 core

layout(location = 0) out vec4 fragColor;
in vec2 v_texCoord;

uniform float u_Scale; // default 1 min 0 max 5
uniform int u_NbIter; // default 2 min 1 max 10
uniform float u_Radius; // default 0.5 min 0 max 3
uniform float u_Thickness; // default 0.01 min 0 max 3
uniform float u_TransAmount; // default 0.2 min 0 max 3
uniform float u_ScaleAmount; // default 0.3333 min 0 max 1
uniform vec2 u_center; // POINT2D default 0 0 
uniform vec3 u_Color; // default 1.0 1.0 1.0

float TAU = 6.28;

vec2 fromAngle(float theta){
	return vec2(cos(theta), sin(theta));
}

void main() {
	vec2 uv = v_texCoord / u_Scale;
	vec3 color;

	float transAmount = u_TransAmount;
	float scaleAmount = u_ScaleAmount;
	for (int k = 0; k < u_NbIter; ++k){
		float angle = atan(uv.y, uv.x); // 0 is at bottom
		if (-TAU/2. < angle && angle < -TAU/2. + TAU/3. ){
			color.r = 1.0;
			uv -= fromAngle( 2*TAU/3. ) * transAmount;
			uv /= scaleAmount;
		}
		else if (angle < -TAU/2. + 2.0*TAU/3.0){
			color.g = 1.0;
			uv -= fromAngle( 0 ) * transAmount;
			uv /= scaleAmount;
		}
		else{
			color.b = 1.0;
			uv -= fromAngle( TAU/3.0 ) * transAmount;
			uv /= scaleAmount;
		}
	}

	color = vec3(0.);
	float alpha = smoothstep(u_Radius + u_Thickness/2. , u_Radius - u_Thickness/2, length(uv-u_center));
	fragColor = vec4(color,alpha);
}