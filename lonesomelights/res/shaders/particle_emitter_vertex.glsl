#version 330

uniform float u_current_time_seconds;

layout(location = 0) in vec3 in_base_position;

layout(location = 1) in vec3 in_instance_start_position;
layout(location = 2) in vec3 in_instance_start_velocity;
layout(location = 3) in vec3 in_instance_acceleration;
layout(location = 4) in float in_instance_start_time_seconds;
layout(location = 5) in float in_instance_lifetime_seconds;

out float pass_time_seconds;
out float pass_instance_lifetime_seconds;

void main() {
	pass_time_seconds = u_current_time_seconds - in_instance_start_time_seconds;
	pass_instance_lifetime_seconds = in_instance_lifetime_seconds;
	
	vec3 position = in_instance_start_position + 0.5 * in_instance_acceleration * pass_time_seconds * pass_time_seconds + in_instance_start_velocity * pass_time_seconds + in_base_position;
	gl_Position = vec4(position, 1.0);
}