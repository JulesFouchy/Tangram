#version 440 core
layout(location = 0) out vec4 FragColor;
in vec2 v_texCoord;
uniform sampler2D TNGu_textureSlot;

// Uniforms
uniform float u_Float; // default 0.5 min 0 max 1
uniform vec3 u_Color; // default 1 0 0


// Main
void main() {
	vec4 texColor = texture(TNGu_textureSlot, v_texCoord);
	vec3 color;
	float alpha;



	FragColor = vec4(color, alpha);
}