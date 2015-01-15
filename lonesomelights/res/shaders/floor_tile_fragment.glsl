#version 330

uniform sampler2D u_texture;

in vec2 pass_texel;
in float pass_brightness;

out vec4 out_color;

void main() {
	vec3 light = normalize(vec3(0.0, 1.0, -1.0));
	vec3 normal = vec3(0.0, 1.0, 0.0);

	//out_color = vec4(0.0, 0.6, 0.1, 1.0);
	out_color.rgb = 0.3 + 0.7 * texture(u_texture, pass_texel).rgb;
	out_color.rgb = vec3(1.0) - out_color.rgb;
	out_color.rgb *= 0.3;
	out_color.rgb *= max(0.0, dot(normal, light));
	//out_color.rgb *= 0.8 + 0.2 * pass_brightness;
}