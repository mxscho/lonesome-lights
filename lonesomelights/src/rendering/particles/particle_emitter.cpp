#include "rendering/particles/particle_emitter.h"

#include "rendering/opengl/render_program.h"
#include "timer.h"

#include <iostream>

ParticleEmitter::ParticleEmitter(const glm::vec3& position, const glm::vec3& particle_start_velocity, const glm::vec3& particle_acceleration, float min_particle_lifetime_seconds, float max_particle_lifetime_seconds, float frequency, float current_time_seconds, unsigned int max_particle_count)
	: Drawable(),
	m_position(position),
	m_particle_start_velocity(particle_start_velocity),
	m_particle_acceleration(particle_acceleration),
	m_min_particle_lifetime_seconds(min_particle_lifetime_seconds),
	m_max_particle_lifetime_seconds(max_particle_lifetime_seconds),
	m_frequency(frequency),
	m_max_particle_count(max_particle_count),
	m_base_vertex_buffer_object({ glm::vec3(0.0F, 0.0F, 0.0F) }, GL_ARRAY_BUFFER),
	m_instances_vertex_buffer_object(m_max_particle_count, GL_ARRAY_BUFFER),
	m_particles(),
	m_next_emission_time_seconds(current_time_seconds),
	m_vertex_array_object() {
	
	m_vertex_array_object.bind();
	m_base_vertex_buffer_object.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
	m_instances_vertex_buffer_object.bind();
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((0 * 3 + 0 * 1) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((1 * 3 + 0 * 1) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((2 * 3 + 0 * 1) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((3 * 3 + 0 * 1) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle::Data), (void*) ((3 * 3 + 1 * 1) * sizeof(GL_FLOAT)));
	VertexBufferObjects::unbind_any();
	VertexArrayObject::unbind_any();
}

void ParticleEmitter::draw(const RenderProgram& render_program) const {
	Drawable::draw(render_program);
	
	render_program.bind();
	
	int uniform_location = glGetUniformLocation(render_program.get_id(), "u_current_time_seconds");
	glUniform1f(uniform_location, m_current_time_seconds);
	
	m_vertex_array_object.bind();

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glPointSize(15.0F);
	glDrawArraysInstanced(GL_POINTS, 0, 1, m_max_particle_count);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
}
void ParticleEmitter::update(const Timer& timer) {
	Updatable::update(timer);
	
	m_current_time_seconds = timer.get_current_time_seconds();
	
	for (auto i_particle = m_particles.begin(); i_particle != m_particles.end(); ) {
		if (i_particle->is_alive(m_current_time_seconds)) {
			++i_particle;
		} else {
			i_particle = m_particles.erase(i_particle);
		}
	}
	
	while (m_next_emission_time_seconds <= m_current_time_seconds) {
		assert(m_particles.size() < m_max_particle_count);
		m_particles.push_back(Particle(m_instances_vertex_buffer_object.claim_bucket(), m_position, m_particle_start_velocity, m_particle_acceleration, m_next_emission_time_seconds, m_min_particle_lifetime_seconds /* TODO: Random lifetime. */));
		m_next_emission_time_seconds += 1.0F / m_frequency;
	}
	
	m_instances_vertex_buffer_object.update();
}