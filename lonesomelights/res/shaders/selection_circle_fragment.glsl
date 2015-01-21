#version 330

uniform sampler2D u_texture;

in vec2 pass_texel;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = vec4(texture(u_texture, pass_texel).rgb, 1.0);
}