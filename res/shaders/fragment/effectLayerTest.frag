#version 440 core

layout(location = 0) out vec4 FragColor;
in vec2 v_texCoord;
uniform sampler2D u_textureSlot;

uniform float u_Thresh; // default 0.5 min 0 max 1
uniform float u_Margin; // default 0.1 min 0 max 0.2
uniform vec3 u_ColorToRemove; // default 0 1 0

void main() {
	vec4 texColor = texture(u_textureSlot, v_texCoord);
	vec3 color = texColor.rgb;
	float alpha = smoothstep(u_Thresh-u_Margin, u_Thresh + u_Margin, length(texColor.rgb - u_ColorToRemove));
	FragColor = vec4(color, alpha);
}