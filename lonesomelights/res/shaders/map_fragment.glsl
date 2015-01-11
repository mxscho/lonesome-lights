#version 330

uniform sampler2D u_texture;

in vec2 pass_texture_coordinates;

out vec4 out_color;

void main() {
	out_color = vec4(texture(u_texture, pass_texture_coordinates).rgb, 1.0);
}