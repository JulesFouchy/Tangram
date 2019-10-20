#version 440 core

layout(location = 0) out vec4 FragColor;
in vec2 v_texCoord;
uniform sampler2D u_textureSlot;

uniform float u_Thresh; // default 0.5 min 0 max 1
uniform float u_Margin; // default 0.1 min 0 max 0.2
uniform vec3 u_ColorToRemove; // default 0 1 0

float computeHue(vec3 col){
	float m = min(min(col.r, col.g), col.b);
	float M = max(max(col.r, col.g), col.b);
	float hue;
	if( col.r == M )
		hue = (col.g - col.b) / (M-m) / 6.;
	else if( col.g == M )
		hue = 1./3. + (col.b - col.r) / (M-m) / 6.;
	else
		hue = 2./3. + (col.r - col.g) / (M-m) / 6.;
	if (hue < 0.)
		hue += 1.;
	return hue;

}

void main() {
	vec4 texColor = texture(u_textureSlot, v_texCoord);
	vec3 color = texColor.rgb;
	//float hue = computeHue(color);
	//float hueToRemove = computeHue(u_ColorToRemove);
	float alpha = texColor.a * smoothstep(u_Thresh-u_Margin, u_Thresh + u_Margin, length(texColor.rgb - u_ColorToRemove));
	//color = vec3(texColor.r+texColor.g+texColor.b)/3.;
	FragColor = vec4(color, alpha);
}