#version 330

in float pass_time_seconds;
in float pass_instance_lifetime_seconds;

out vec4 out_color;

void main() {
	if (pass_instance_lifetime_seconds <= pass_time_seconds) {
		discard;
	}

	out_color = vec4(1.0, 1.0 - pass_time_seconds / pass_instance_lifetime_seconds, 0.0, 1.0);
}