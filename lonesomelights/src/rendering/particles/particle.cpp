#include "rendering/particles/particle.h"

#include <iostream>

Particle::Data::Data(const glm::vec3& start_position, const glm::vec3& start_velocity, const glm::vec3& acceleration, float start_time_seconds, float lifetime_seconds)
	: start_position(start_position),
	start_velocity(start_velocity),
	acceleration(acceleration),
	start_time_seconds(start_time_seconds),
	lifetime_seconds(lifetime_seconds) {
}
Particle::Data::Data()
	: start_position(),
	start_velocity(),
	acceleration(),
	start_time_seconds(),
	lifetime_seconds() {
}

Particle::Particle(VertexBufferObject<Particle::Data>::Bucket&& vertex_buffer_object_bucket, const glm::vec3& start_position, const glm::vec3& start_velocity, const glm::vec3& acceleration, float start_time_seconds, float lifetime_seconds)
	: m_vertex_buffer_object_bucket(std::move(vertex_buffer_object_bucket)) {
	m_vertex_buffer_object_bucket.set(Particle::Data(start_position, start_velocity, acceleration, start_time_seconds, lifetime_seconds));
}

bool Particle::is_alive(float current_time) const {
	const Particle::Data& data = m_vertex_buffer_object_bucket.get();
	return data.start_time_seconds + data.lifetime_seconds > current_time;
}