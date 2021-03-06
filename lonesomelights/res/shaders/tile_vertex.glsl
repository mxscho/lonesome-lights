#version 330

uniform mat4 u_model_transformation;
uniform mat4 u_view_transformation;
uniform mat4 u_projection_transformation;
uniform vec3 u_camera_eye_position;
uniform vec3 u_camera_up_direction;

layout(location = 0) in vec3 in_position;

void main() {
	gl_Position = u_projection_transformation * u_view_transformation * u_model_transformation * vec4(in_position, 1.0);
}
