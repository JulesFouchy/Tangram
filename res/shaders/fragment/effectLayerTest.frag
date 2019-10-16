#version 440 core

layout(location = 0) out vec4 FragColor;

in vec2 v_texCoord;

uniform sampler2D u_textureSlot;

void main() {
	vec4 texColor = texture(u_textureSlot, v_texCoord);
	vec3 color = vec3((texColor.r+texColor.g+texColor.b)/3.);
	float alpha = texColor.a;
	FragColor = vec4(color, alpha);
}