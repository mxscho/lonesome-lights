#version 330

uniform mat4 u_model_transformation;
uniform mat4 u_view_transformation;
uniform mat4 u_projection_transformation;
uniform vec3 u_camera_eye_position;
uniform vec3 u_camera_up_direction;

layout(location = 0) in vec3 in_position;

out vec3 pass_position;
out vec2 pass_texel;

float rand(vec2 seed){
    return fract(sin(dot(seed.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
float rand(float seed_a, float seed_b){
    return rand(vec2(seed_a, seed_b));
}

void main() {
	vec4 position = u_model_transformation * vec4(in_position, 1.0);
	pass_position = position.xyz;
	gl_Position = u_projection_transformation * u_view_transformation * position;
	
	pass_texel = ((u_model_transformation * vec4(in_position.x, 0.0F, in_position.z, 1.0)) / 4.0F).xz;
}