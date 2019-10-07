#version 440 core

layout(location = 0) out vec4 fragColor;
in vec2 v_texCoord;

uniform float u_m; //default 0.029 min 0 max 0.05
uniform float u_n; //default 0.009 min 0 max 0.03
//uniform float u_QuasiIdentityPow; // default 1 min 0 max 5
//uniform float u_QuasiIdentityAmplitude; // default 1 min 0 max 2
uniform float u_ScaleX; //default 1 min 0 max 10
//uniform int u_NbIterNewton; //default 10 min 0 max 50
//uniform float u_NewtonInitialPoint; // default 0.025 min 0 max 0.1
//uniform float u_Pow; // default 1 min 0 max 1
uniform float u_Thickness; // default 0.009 min 0 max 0.3
uniform float u_Length; // default 0.571 min 0 max 2.
uniform float u_EndLine; // default 0 min 0 max 0.15
uniform float u_WingPow; // default 0.536 min 0 max 5
uniform float u_PowScale; // default 2.857 min 0 max 20
//uniform float u_smoothMinPow; // default 1.525 min 0 max 20
uniform vec2 u_Position; // default 0 0
uniform vec3 u_Color; // default 0 0 0

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

/*float f(float x, vec2 uv){
	return 2.*x - 2.*uv.x + 2*u_Pow*pow(x,2*u_Pow-1.) - 2.*u_Pow*uv.y*pow(x, u_Pow-1.);
}
float df(float x, vec2 uv){
	return 2. + 2*u_Pow*(2.*u_Pow-1.)*pow(x,2*u_Pow-2.) - 2.*u_Pow*(u_Pow-1.)*uv.y*pow(x, u_Pow-2.);
}
float newton(vec2 uv){
	float x0 = u_NewtonInitialPoint;
	for( int i = 0; i < u_NbIterNewton; ++i ){
		x0 -= f(x0, uv)/df(x0, uv);
	}
	return x0;
}
float distToCurve(vec2 uv){
	float xMin = clamp(newton(uv), 0., u_Length);
	return length( uv - vec2(xMin, pow(xMin,u_Pow)) );
}*/

//float quasiIdentity(float x){
	//return x + u_QuasiIdentityAmplitude * exp(-pow(x,u_QuasiIdentityPow));
//}

float almostIdentity( float x, float m, float n )
{
    if( x>m ) return x;
    float a = 2.0*n - m;
    float b = 2.0*m - 3.0*n;
    float t = x/m;
    return (a*t + b)*t*t + n;
}

float line(vec2 uv){
	vec2 dir = normalize(vec2(1.0));
	float proj = dot(uv, dir);
	float U = proj;
	float V = length(uv - proj * dir);
	float t = smoothstep(u_Thickness, 0., abs(V));
	t *= smoothstep(u_Length, 0., U) - smoothstep(0., -u_EndLine, U);
	return t;
}

void main() {
	vec2 uv = v_texCoord;
	vec3 color = u_Color;

	uv.x = almostIdentity(abs(uv.x), u_m, u_n);
	uv.x /= u_ScaleX;
	uv -= u_Position;
	
	uv.y *= pow(uv.x*u_PowScale,u_WingPow);

	float t = max( line(uv), line(uv*vec2(1.,1.))) ;
	//float t1 = smoothstep(u_Thickness, 0., distToCurve(uv));
	//uv.x*=-1.;
	//float t2 = smoothstep(u_Thickness, 0., distToCurve(uv));
	//float t = max(t1,t2);

	fragColor = vec4(color, t);
}