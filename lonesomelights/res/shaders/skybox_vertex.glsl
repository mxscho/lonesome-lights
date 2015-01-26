#version 330

uniform mat4 u_model_transformation;
uniform mat4 u_view_transformation;
uniform mat4 u_projection_transformation;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texel;

out vec3 pass_position;
out vec2 pass_texel;

void main() {
	vec3 position = (u_model_transformation * vec4(in_position, 1.0)).xyz;
	gl_Position = u_projection_transformation * u_view_transformation * vec4(position, 1.0);
	pass_position = position;
	pass_texel = in_texel;
}
