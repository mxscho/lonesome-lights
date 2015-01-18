#version 330

uniform mat4 u_model_transformation;
uniform mat4 u_view_transformation;
uniform mat4 u_projection_transformation;
uniform vec3 u_camera_eye_position;
uniform vec3 u_camera_up_direction;

uniform bool u_orientate_towards_velocity;
uniform float u_current_time_seconds;

layout(location = 0) in vec3 in_base_position;

layout(location = 1) in vec3 in_instance_start_position;
layout(location = 2) in vec3 in_instance_start_velocity;
layout(location = 3) in vec3 in_instance_acceleration;
layout(location = 4) in float in_instance_start_time_seconds;
layout(location = 5) in float in_instance_lifetime_seconds;

out float pass_local_time_seconds;
out float pass_instance_lifetime_seconds;
out vec3 pass_instance_position;

out vec2 pass_texel;

mat4 create_rotation(vec3 axis, float angle) {
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,  oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c,          oc * axis.y * axis.z - axis.x * s, 0.0,
		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c,          0.0,
		0.0,                               0.0,                               0.0,                               1.0);
}

void main() {
	float local_time_seconds = u_current_time_seconds - in_instance_start_time_seconds;

	vec3 instance_position = in_instance_start_position + 0.5 * in_instance_acceleration * local_time_seconds * local_time_seconds + in_instance_start_velocity * local_time_seconds;
	vec3 instance_position_world = (u_model_transformation * vec4(instance_position, 1.0)).xyz;
	
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
	vec3 billboard_position = (billboard_rotation * vec4(in_base_position, 1.0)).xyz;
	
	gl_Position = u_projection_transformation * u_view_transformation * vec4(instance_position_world + billboard_position, 1.0);
	
	pass_local_time_seconds = local_time_seconds;
	pass_instance_lifetime_seconds = in_instance_lifetime_seconds;
	pass_instance_position = instance_position;
	
	pass_texel = (in_base_position.xy / abs(in_base_position.xy) + 1.0) / 2.0;
}