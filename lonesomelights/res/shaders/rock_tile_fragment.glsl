#version 330

uniform sampler2D u_texture;

out vec4 out_color;

in vec3 pass_normal;
in vec2 pass_texel;

void main() {
	vec3 light = normalize(vec3(-0.5, 1.0, -0.5));

	vec3 normal = normalize(pass_normal);

	//out_color = vec4(0.8, 0.3, 0.0, 1.0);
	out_color.rgb = texture(u_texture, pass_texel).rgb;
	out_color *= max(0.0, dot(normal, light));
}