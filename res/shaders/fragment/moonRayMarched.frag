#version 440 core

layout(location = 0) out vec4 fragColor;
in vec2 v_texCoord;

uniform float u_FocalLength; // default 0.26 min 0 max 1
uniform float u_MinDistToBreak; // default 0.00 min 0 max 0.01
uniform float u_Exposure; // default 4.855 min 0 max 5

uniform float u_MoonRadius; //default 0.225 min 0 max 1
uniform float u_NoiseAmpl; //default 0.002 min 0 max 0.002
uniform float u_NoiseScale; //default 0.046 min 0 max 0.5
uniform float u_NoisePow; //default 1.171 min 0 max 10
uniform float u_AmplDecay; //default 0.4 min 0 max 1
uniform float u_ScaleDecay; //default 0.488 min 0 max 1
uniform float u_ShadowPow; //default 1.688 min 0 max 2
uniform float u_CrateParam; //default 0.1 min 0 max 2
uniform float u_CrateRadius; //default 0.1 min 0 max 2
uniform vec3 u_CrateDir; // NOT_A_COLOR default -0.733 0.467 0.191 min -1 -1 -1 max 1 1 1
uniform vec3 u_SunDir; // NOT_A_COLOR default -0.733 0.467 0.191 min -1 -1 -1 max 1 1 1
uniform vec3 u_MoonColour ;  //default 0.528f, 0.380f, 0.223f
uniform vec3 u_SunColor ;  //default 1 1 1
uniform vec3 u_SpaceColour ;  //default 0 0 0

float smoothMin( float a, float b, float k )
{//https://www.iquilezles.org/www/articles/smin/smin.htm
    float res = exp2( -k*a ) + exp2( -k*b );
    return -log2( res )/k;
}

//
// Description : Array and textureless GLSL 2D/3D/4D simplex
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
  {
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i);
  vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                dot(p2,x2), dot(p3,x3) ) );
  }

  float fractalNoise( vec3 pos ){
		float ampl = 1.0;
		float scale = 1.0;
		float res = 0;
		for( int i = 0; i < 5; ++i ){
			res += ampl * snoise( pos / scale );
			ampl *= u_AmplDecay;
			scale *= u_ScaleDecay;
		}
		return res;
  }

 float SDFsphere( vec3 pos ){
	return length(pos) - u_MoonRadius;
}

float angleBetween( vec3 v1, vec3 v2 ){
	return acos(dot(v1,v2)/length(v1)/length(v2));
}

float expImpulse( float k, float x )
{
    const float h = k*x;
    return h*exp(1.0-h);
}

float SDF( vec3 pos ){
	float d1 = SDFsphere(pos) + u_NoiseAmpl*pow(fractalNoise(pos/u_NoiseScale), u_NoisePow);
	float angle = angleBetween( u_CrateDir, pos );
	float hCratere = expImpulse(u_CrateParam, angle - u_CrateRadius);
	return d1;
}

vec3 getNormal( vec3 pos ){
	vec2 e = vec2(0.001,0.0);
	return normalize(vec3( SDF(pos+e.xyy) - SDF(pos),
	                       SDF(pos+e.yxy) - SDF(pos),
	                       SDF(pos+e.yyx) - SDF(pos)

	));
}

vec3 getNormalSphere( vec3 pos ){
	vec2 e = vec2(0.001,0.0);
	return normalize(vec3( SDFsphere(pos+e.xyy) - SDFsphere(pos),
	                       SDFsphere(pos+e.yxy) - SDFsphere(pos),
	                       SDFsphere(pos+e.yyx) - SDFsphere(pos)

	));
}

float castRay(vec3 ro, vec3 rd){	
	float t = 0.0;
	for (int i = 0; i < 100; ++i){
		vec3 pos = ro + t*rd;
		float dist = SDF( pos );
		if( dist < u_MinDistToBreak )
			break;
		t += dist;
		if( t > 20.0 )
			break;
	}
	if( t > 20. ) 
		t = -1.;
	return t;
}

float castRaySphere(vec3 ro, vec3 rd){	
	float t = 0.0;
	for (int i = 0; i < 100; ++i){
		vec3 pos = ro + t*rd;
		float dist = SDFsphere( pos );
		if( dist < u_MinDistToBreak )
			break;
		t += dist;
		if( t > 20.0 )
			break;
	}
	if( t > 20. ) 
		t = -1.;
	return t;
}

void main() {
	vec2 uv = v_texCoord;
    vec3 col = u_SpaceColour;

	vec3 ro = vec3(0., 0., u_FocalLength);
	vec3 rd = normalize( vec3(uv,0.0) - ro );

	float tSphere = castRaySphere(ro, rd);
	float t = castRay(ro, rd);
	if( t > 0.0 ){
		col = u_MoonColour;
		vec3 posSphere = ro + tSphere*rd;
		vec3 normalSphere = getNormalSphere( posSphere );
		vec3 pos = ro + t*rd;
		vec3 normal = getNormal( pos );

		float shadow = clamp( 0., 1., -castRay( pos + 0.001 * normal, u_SunDir ));

		float sun_dif = clamp( dot( normalize(u_SunDir),normal) ,0.0, 1.0);
		
		col *= u_Exposure * u_SunColor * sun_dif * shadow;//pow( shadow, u_ShadowPow);
		//col += vec3(0.01);
	}
	else 
		discard;

	col = pow(col, vec3(0.4545));
    fragColor = vec4(col, 1.0);
}