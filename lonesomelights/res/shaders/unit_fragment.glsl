#version 330

uniform vec3 u_color;

out vec4 out_color;

in vec3 pass_normal;

void main() {
	vec3 light = normalize(vec3(-0.5, 1.0, -0.5));

	vec3 normal = normalize(pass_normal);

	out_color = vec4(u_color, 1.0);
	out_color *= max(0.0, dot(normal, light));
}