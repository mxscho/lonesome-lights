#version 330

uniform mat4 u_model_transformation;
uniform mat4 u_view_transformation;
uniform mat4 u_projection_transformation;

layout(location = 0) in vec3 in_position;

out vec2 pass_texture_coordinates;

void main() {
	gl_Position = u_projection_transformation * u_view_transformation * u_model_transformation * vec4(in_position, 1.0);
	pass_texture_coordinates = in_position.xz;
}