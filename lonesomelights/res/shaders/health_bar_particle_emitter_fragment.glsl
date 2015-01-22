#version 330

uniform sampler2D u_texture;
uniform float u_max_health;
uniform float u_health;

in float pass_local_time_seconds;
in float pass_instance_lifetime_seconds;
in vec3 pass_instance_position;

in vec2 pass_texel;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normal;

void main() {
	if (pass_instance_lifetime_seconds <= pass_local_time_seconds || length(pass_instance_position) < 0) {
		discard;
	}

	float health = u_health / u_max_health;
	
	out_color = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 texture_color = texture(u_texture, pass_texel).rgba;
	out_color.rgb = texture_color.a * texture_color.rgb;
	if (pass_texel.x <= health) {
		out_color.rgb += (1.0 - texture_color.a) * (health * vec3(0.0, 1.0, 0.0) + (1.0 - health) * vec3(1.0, 0.0, 0.0));
	} else {
		out_color.rgb += (1.0 - texture_color.a) * vec3(0.0, 0.0, 0.0);
	}
	
	out_position = vec4(0.0);
	out_normal = vec4(0.0);
}