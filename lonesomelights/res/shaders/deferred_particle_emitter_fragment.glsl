#version 330

uniform sampler2D u_color_texture;
uniform sampler2D u_position_texture;
uniform sampler2D u_normal_texture;
uniform sampler2D u_depth_texture;

in vec2 pass_deferred_texel;

uniform sampler2D u_texture;
uniform vec3 u_start_color;
uniform vec3 u_end_color;
uniform float u_lighting_intensity;

in float pass_local_time_seconds;
in float pass_instance_lifetime_seconds;
in vec3 pass_instance_position;
in vec3 pass_instance_position_world;
in vec2 pass_texel;
in float pass_lighting_radius;

layout(location = 0) out vec4 out_color;

void main() {
	if (pass_instance_lifetime_seconds <= pass_local_time_seconds || length(pass_instance_position) < 0) {
		discard;
	}

	vec3 world_position_to_instance_position = pass_instance_position_world - texture(u_position_texture, pass_deferred_texel).rgb;
	float world_position_to_instance_position_length = length(world_position_to_instance_position);
	vec3 world_normal = normalize(texture(u_normal_texture, pass_deferred_texel).rgb);
	float lighting_factor = /*max(0.0, dot(world_normal, normalize(world_position_to_instance_position))) * */(1.0 - min(1.0, world_position_to_instance_position_length / pass_lighting_radius));
	
	float time_factor = pass_local_time_seconds / pass_instance_lifetime_seconds;
	out_color = vec4((1.0 - time_factor) * u_start_color + time_factor * u_end_color, 1.0);
	out_color.rgb *= u_lighting_intensity;
	out_color.rgb *= lighting_factor;
	if (time_factor >= 0.9) {
		out_color.rgb *= 1.0 - (time_factor - 0.9) / 0.1;
	} else if (time_factor < 0.1) {
		out_color.rgb *= time_factor / 0.1;
	}
}