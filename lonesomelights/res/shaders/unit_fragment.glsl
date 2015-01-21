#version 330

uniform vec3 u_color;

in vec3 pass_position;
in vec3 pass_normal;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normal;

void main() {
	vec3 light = normalize(vec3(-0.5, 1.0, -0.5));

	vec3 normal = normalize(pass_normal);

	out_color = vec4(u_color, 1.0);
	out_color *= max(0.0, dot(normal, light));
	
	out_position = vec4(pass_position, 1.0);
	
	out_normal = vec4(normal, 0.0);
}