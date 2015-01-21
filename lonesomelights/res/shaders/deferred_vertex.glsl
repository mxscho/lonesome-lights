#version 330

layout(location = 0) in vec2 in_position;

out vec2 pass_texel;

void main() {
	gl_Position = vec4(in_position, 0.0, 1.0);
	
	pass_texel = (in_position + vec2(1.0, 1.0)) / 2.0;
}