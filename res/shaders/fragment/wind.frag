#version 440 core

layout(location = 0) out vec4 fragColor;

uniform float u_scale; // default 0.755 min 0 max 25
uniform vec2 u_offset; // default 0 0 min -2 -2 max 2 2 
uniform float u_thickness; // default 0.153 min 0 max 0.2
uniform float u_nbLines; // default 30 min 0 max 50
uniform float u_coneShape; // default 0.785 min 0 max 10
uniform float u_rapprochementLignes; // default 0.532 min 0 max 2
uniform float u_noiseAmplitude; // default 0.012 min 0 max 2
uniform float u_decayThickness; // default 11.662 min 0 max 20
uniform float u_noiseScale; // default 1 min 0 max 2
uniform vec2 u_noiseOffset; // default 0 0 min -5 -5 max 5 5
uniform float u_pointille; // default 5 min 0 max 20
uniform float u_pointilleYvariation; // default 1 min 0 max 2
uniform float u_powPointille; // default 1 min 0 max 5	
uniform vec3 u_Color; // default (0.075f, 0.359f, 0.636f)

in vec2 v_texCoord;

//
// Description : Array and textureless GLSL 2D simplex noise function.
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

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
    + i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

void main() {
	vec2 uv = v_texCoord + u_offset;
	uv /= u_scale;
	float cosArg = uv.x +pow(snoise(u_pointilleYvariation * vec2(uv.y)),u_powPointille);
	uv.y /= exp(u_coneShape * uv.x);
	uv.y = pow(uv.y,u_rapprochementLignes);
	uv.y += u_noiseAmplitude * snoise(u_noiseScale*(uv+u_noiseOffset));

	float thickness = u_thickness * exp(-u_decayThickness * uv.y);
	uv.y = fract(uv.y*u_nbLines/2. +0.5)-0.5;
	vec3 color = u_Color;
	float alpha = 1.0;
	alpha = (smoothstep(-thickness,0,uv.y) - smoothstep(0, thickness, uv.y)) * (cos(u_pointille * (cosArg)+snoise(uv*u_noiseScale))+1.)/2.;

	fragColor = vec4(color, alpha);
}