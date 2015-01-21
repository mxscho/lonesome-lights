#version 330

uniform sampler2D u_texture_color;
uniform sampler2D u_texture_depth;

in vec2 pass_texel;

out vec4 out_color;

void main() {
	out_color = vec4(texture(u_texture_color, pass_texel).rgb, 1.0);
	gl_FragDepth = texture(u_texture_depth, pass_texel);
}