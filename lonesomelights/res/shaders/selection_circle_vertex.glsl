#version 330

uniform mat4 u_model_transformation;
uniform mat4 u_view_transformation;
uniform mat4 u_projection_transformation;
uniform vec3 u_camera_eye_position;
uniform vec3 u_camera_up_direction;

layout(location = 0) in vec2 in_position;

out vec2 pass_texel;

void main() {
	gl_Position = u_projection_transformation * u_view_transformation * u_model_transformation * vec4(in_position.x, 0.0, in_position.y, 1.0);
	pass_texel = in_position + vec2(0.5F);
}
