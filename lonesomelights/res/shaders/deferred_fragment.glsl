#version 330

uniform sampler2D u_color_texture;
uniform sampler2D u_position_texture;
uniform sampler2D u_normal_texture;
uniform sampler2D u_depth_texture;

in vec2 pass_texel;

out vec4 out_color;

void main() {
	out_color = vec4(texture(u_color_texture, pass_texel).rgb, 1.0);
	gl_FragDepth = texture(u_depth_texture, pass_texel).r;

	/* DEBUG
	if (pass_texel.x < 0.5 && pass_texel.y < 0.5) {
		out_color = vec4(texture(u_color_texture, pass_texel * 2.0).rgb, 1.0);
		gl_FragDepth = texture(u_depth_texture, pass_texel * 2.0).r;
	} else if (pass_texel.x >= 0.5 && pass_texel.y < 0.5) {
		out_color = vec4(texture(u_position_texture, vec2(pass_texel.x * 2.0 - 1.0, pass_texel.y * 2.0)).rgb, 1.0);
		gl_FragDepth = texture(u_depth_texture, vec2(pass_texel.x * 2.0 - 1.0, pass_texel.y * 2.0)).r;
	} else if (pass_texel.x < 0.5 && pass_texel.y >= 0.5) {
		out_color = vec4(texture(u_normal_texture, vec2(pass_texel.x * 2.0, pass_texel.y * 2.0 - 1.0)).rgb, 1.0);
		gl_FragDepth = texture(u_depth_texture, vec2(pass_texel.x * 2.0, pass_texel.y * 2.0 - 1.0)).r;
	} else if (pass_texel.x >= 0.5 && pass_texel.y >= 0.5) {
		out_color = vec4(texture(u_depth_texture, pass_texel * 2.0 - vec2(1.0)).rgb, 1.0);
		gl_FragDepth = texture(u_depth_texture, pass_texel * 2.0 - vec2(1.0)).r;
	}
	*/
}