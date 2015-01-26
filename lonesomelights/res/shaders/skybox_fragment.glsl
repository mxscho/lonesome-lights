#version 330

uniform vec3 u_color;
uniform sampler2D u_texture;

in vec3 pass_position;
in vec2 pass_texel;

layout(location = 0) out vec4 out_color;

void main() {

	//out_color = vec4(u_color, 1.0);
	out_color.rgb = texture(u_texture, pass_texel).rgb;
	//out_color.rg = pass_texel;
}
