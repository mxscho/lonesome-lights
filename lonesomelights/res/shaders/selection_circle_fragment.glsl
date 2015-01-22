#version 330

uniform sampler2D u_texture;
uniform vec3 u_color;

in vec2 pass_texel;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = vec4(u_color, 1.0);
	out_color.rgb *= texture(u_texture, pass_texel).rgb;
}