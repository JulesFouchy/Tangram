#version 440 core

layout(location = 0) out vec4 fragColor;

uniform float u_wave1; //default 0.1 min 0 max 1
uniform float u_pulsWave1; //default 12 min 0 max 50
uniform float u_powWave1; // default 5 min -1 max 25
uniform float u_wave2; //default 0.1 min 0 max 1
uniform float u_pulsWave2; //default 12 min 0 max 50
uniform float u_powWave2; // default 5 min -1 max 25

uniform float u_radius1; // default 0.133 min 0 max 1.5
uniform vec2 u_center1; //default -1.2 0.286 min -2 -2 max 2 2
uniform vec3 u_Color1; // default 0.975, 0.733, 0.196

uniform float u_radius2; // default 0.389 min 0 max 1.5
uniform vec2 u_center2; //default 1.086 0 min -2 -2 max 2 2
uniform vec3 u_Color2; // default 0.949 0.495 0.234 

uniform float u_smoothMinPow; // default 1.525 min -1 max 20
uniform float u_atanScale; // default 9.633 min -1 max 20

uniform vec4 u_BackColour ;  //default 0.093, 0.000, 0.43, 0.000

in vec2 v_texCoord;

float smoothMin( float a, float b, float k )
{//https://www.iquilezles.org/www/articles/smin/smin.htm
    float res = exp2( -k*a ) + exp2( -k*b );
    return -log2( res )/k;
}

void main() {
	vec2 uv = v_texCoord;
	float PI = 3.141592653;
    
	float angle1 = atan( (uv - u_center1).y, (uv - u_center1).x);
	float angle2 = atan( (uv - u_center2).y, (uv - u_center2).x);

	float d1 = length( uv - u_center1 ) - u_radius1 + u_wave1*pow(cos(angle1*u_pulsWave1), u_powWave1);
    float d2 = length( uv - u_center2 ) - u_radius2 + u_wave2*pow(cos(angle2*u_pulsWave2), u_powWave2);
    float d = smoothMin(d1,d2,u_smoothMinPow);
    // col1 = vec3(0.885,0.417,0.349);
    //vec3 col2 = vec3(0.725,0.447,0.885);
    
    float blendFactor = atan(d*u_atanScale)/PI*2.0;
    vec4 col = vec4(d1/(d1+d2) * u_Color1 + d2/(d1+d2) * u_Color2, 1.0);

    fragColor = (1.0-blendFactor) * col  + blendFactor * u_BackColour;
}