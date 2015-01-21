#version 330

uniform sampler2D u_texture;
uniform vec3 u_start_color;
uniform vec3 u_end_color;

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

	float time_factor = pass_local_time_seconds / pass_instance_lifetime_seconds;
	out_color = vec4((1.0 - time_factor) * u_start_color + time_factor * u_end_color, 1.0);
	out_color *= vec4(texture(u_texture, pass_texel).rgb, 1.0);
	if (time_factor >= 0.9) {
		out_color.rgb *= 1.0 - (time_factor - 0.9) / 0.1;
	} else if (time_factor < 0.1) {
		out_color.rgb *= time_factor / 0.1;
	}
	
	out_position = vec4(0.0);
	out_normal = vec4(0.0);
}