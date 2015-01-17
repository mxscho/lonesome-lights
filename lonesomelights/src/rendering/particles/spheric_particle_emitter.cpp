#include "rendering/particles/spheric_particle_emitter.h"

#include <cstdlib>
#include <glm/glm.hpp>

SphericParticleEmitter::SphericParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, float billboard_size, float current_time_seconds,
	float radius, float particle_start_velocity, float particle_acceleration, float min_particle_lifetime_seconds, float max_particle_lifetime_seconds, float frequency)
	: ParticleEmitter(transformation, parent_transformable, billboard_size, current_time_seconds,
		(unsigned int) ((float) frequency * max_particle_lifetime_seconds * 1.1F)
	),
	m_radius(radius),
	m_particle_start_velocity(particle_start_velocity),
	m_particle_acceleration(particle_acceleration),
	m_min_particle_lifetime_seconds(min_particle_lifetime_seconds),
	m_max_particle_lifetime_seconds(max_particle_lifetime_seconds) {
	ParticleEmitter::m_frequency = frequency;
}
void SphericParticleEmitter::recalculate_properties() {
	glm::vec3 emission_direction = glm::normalize(glm::vec3(
		2.0F * (float) rand() / RAND_MAX - 1.0F,
		2.0F * (float) rand() / RAND_MAX - 1.0F,
		2.0F * (float) rand() / RAND_MAX - 1.0F
	));
	ParticleEmitter::m_particle_offset = glm::vec3(m_radius * emission_direction);
	ParticleEmitter::m_particle_start_velocity = m_particle_start_velocity * emission_direction;
	ParticleEmitter::m_particle_acceleration = m_particle_acceleration * emission_direction;
	ParticleEmitter::m_particle_lifetime_seconds = m_min_particle_lifetime_seconds + (float) rand() / RAND_MAX * (m_max_particle_lifetime_seconds - m_min_particle_lifetime_seconds);
}