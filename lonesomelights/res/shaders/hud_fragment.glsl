#version 330

uniform sampler2D u_texture;

in vec2 pass_texel;

layout(location = 0) out vec4 out_color;

void main() {
	vec4 color = texture(u_texture, pass_texel).rgba;
	if (color.a == 0.0) {
		discard;
	}
	out_color = color;
}
