#version 330

uniform sampler2D u_texture;

in vec3 pass_position;
in vec2 pass_texel;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normal;

void main() {
	vec3 light = normalize(vec3(0.0, 1.0, -1.0));
	vec3 normal = vec3(0.0, 1.0, 0.0);

	out_color = vec4(0.0, 0.0, 0.0, 1.0);
	out_color.rgb = 0.3 + 0.7 * texture(u_texture, pass_texel).rgb;
	out_color.rgb = vec3(1.0) - out_color.rgb;
	out_color.rgb *= 0.3;
	out_color.rgb *= max(0.0, dot(normal, light));
	
	out_position = vec4(pass_position, 1.0);
	
	out_normal = vec4(normal, 0.0);
}