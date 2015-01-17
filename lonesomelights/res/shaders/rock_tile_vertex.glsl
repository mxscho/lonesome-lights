#version 330

uniform mat4 u_model_transformation;
uniform mat4 u_view_transformation;
uniform mat4 u_projection_transformation;
uniform vec3 u_camera_eye_position;
uniform vec3 u_camera_up_direction;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texel;

out vec3 pass_normal;
out vec2 pass_texel;

void main() {
	gl_Position = u_projection_transformation * u_view_transformation * u_model_transformation * vec4(in_position, 1.0);
	pass_normal = (u_model_transformation * vec4(in_normal, 0.0)).xyz;
	pass_texel = in_texel;
}
