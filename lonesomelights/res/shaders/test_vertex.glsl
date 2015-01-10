#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_offset;

void main() {
	gl_Position = vec4(in_offset + in_position, 1.0);
}