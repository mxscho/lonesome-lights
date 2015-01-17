#version 330

uniform mat4 u_model_transformation;
uniform mat4 u_view_transformation;
uniform mat4 u_projection_transformation;
uniform vec3 u_camera_eye_position;
uniform vec3 u_camera_up_direction;

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
	
	vec3 instance_position = in_instance_start_position + 0.5 * in_instance_acceleration * pass_time_seconds * pass_time_seconds + in_instance_start_velocity * pass_time_seconds;
	vec3 instance_position_world = (u_model_transformation * vec4(instance_position, 1.0)).xyz;
	
	vec3 camera_position_world = u_camera_eye_position;
	vec3 camera_up_world = normalize(u_camera_up_direction);
	
	vec3 billboard_look_world = normalize(camera_position_world - instance_position_world);
	vec3 billboard_right_world = normalize(cross(camera_up_world, billboard_look_world));
	vec3 billboard_up_world = normalize(cross(billboard_look_world, billboard_right_world));
	
	mat4 billboard_transformation = mat4(
		vec4(billboard_right_world, 0.0),
		vec4(billboard_up_world, 0.0),
		vec4(billboard_look_world, 0.0),
		vec4(instance_position_world, 1.0)
	);
	
	
	
	gl_Position = u_projection_transformation * u_view_transformation * billboard_transformation * vec4(in_base_position, 1.0);
}