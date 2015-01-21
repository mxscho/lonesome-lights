#version 330

uniform sampler2D u_color_texture;
uniform sampler2D u_position_texture;
uniform sampler2D u_normal_texture;
uniform sampler2D u_depth_texture;

out vec2 pass_deferred_texel;

uniform mat4 u_model_transformation;
uniform mat4 u_view_transformation;
uniform mat4 u_projection_transformation;
uniform vec3 u_camera_eye_position;
uniform vec3 u_camera_up_direction;

uniform bool u_orientate_towards_velocity;
uniform float u_current_time_seconds;

layout(location = 0) in vec2 in_base_position;

layout(location = 1) in vec3 in_instance_start_position;
layout(location = 2) in vec3 in_instance_start_velocity;
layout(location = 3) in vec3 in_instance_acceleration;
layout(location = 4) in float in_instance_start_time_seconds;
layout(location = 5) in float in_instance_lifetime_seconds;

out float pass_local_time_seconds;
out float pass_instance_lifetime_seconds;
out vec3 pass_instance_position;
out vec3 pass_instance_position_world;
out vec2 pass_texel;
out float pass_lighting_radius;

void main() {
	float c_lighting_radius = 1.0;

	float local_time_seconds = u_current_time_seconds - in_instance_start_time_seconds;
	
	vec3 instance_position = in_instance_start_position + 0.5 * in_instance_acceleration * local_time_seconds * local_time_seconds + in_instance_start_velocity * local_time_seconds;
	vec3 instance_position_world = (u_model_transformation * vec4(instance_position, 1.0)).xyz;
	
	if (in_instance_lifetime_seconds <= local_time_seconds || length(instance_position) < 0) {
		gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		vec3 camera_position_world = u_camera_eye_position;
		vec3 negative_camera_direction_world = normalize(camera_position_world - instance_position_world);
		
		vec3 billboard_look_world;
		vec3 billboard_right_world;
		vec3 billboard_up_world;
		
		if (u_orientate_towards_velocity) {
			vec3 instance_velocity_direction = normalize(in_instance_start_velocity + in_instance_acceleration * local_time_seconds);
			vec3 instance_velocity_direction_world = normalize((u_model_transformation * vec4(instance_velocity_direction, 0.0)).xyz);
			
			billboard_right_world = instance_velocity_direction_world;
			billboard_up_world = normalize(cross(negative_camera_direction_world, billboard_right_world));
			billboard_look_world = normalize(cross(billboard_right_world, billboard_up_world));
		} else {
			vec3 camera_up_world = normalize(u_camera_up_direction);
		
			billboard_look_world = negative_camera_direction_world;
			billboard_right_world = normalize(cross(camera_up_world, billboard_look_world));
			billboard_up_world = normalize(cross(billboard_look_world, billboard_right_world));
		}	
		
		mat4 billboard_rotation = mat4(
			vec4(billboard_right_world, 0.0),
			vec4(billboard_up_world, 0.0),
			vec4(billboard_look_world, 0.0),
			vec4(0.0, 0.0, 0.0, 1.0)
		);
		vec3 billboard_position = (billboard_rotation * vec4(2.0 * c_lighting_radius * (in_base_position / abs(in_base_position)), 0.0, 1.0)).xyz;
		
		gl_Position = u_projection_transformation * u_view_transformation * vec4(instance_position_world + billboard_position, 1.0);
	}
	pass_local_time_seconds = local_time_seconds;
	pass_instance_lifetime_seconds = in_instance_lifetime_seconds;
	pass_instance_position = instance_position;
	pass_instance_position_world = instance_position_world;
	pass_texel = (in_base_position / abs(in_base_position) + 1.0) / 2.0;
	pass_lighting_radius = c_lighting_radius;
	
	gl_Position /= gl_Position.w;
	pass_deferred_texel = (gl_Position.xy + vec2(1.0)) / 2.0;
}