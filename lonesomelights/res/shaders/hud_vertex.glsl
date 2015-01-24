#version 330

layout(location = 0) in vec2 in_position;

out vec2 pass_texel;

void main() {
	gl_Position = vec4(in_position.x, in_position.y, -0.999, 1.0);
	pass_texel = (in_position + vec2(1.0F)) / 2.0F;
}
