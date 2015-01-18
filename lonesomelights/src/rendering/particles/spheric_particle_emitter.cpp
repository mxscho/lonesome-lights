#include "rendering/particles/spheric_particle_emitter.h"

#include <cmath>
#include <cstdlib>
#include <string>
#include <GL/glew.h>

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/texture.h"

SphericParticleEmitter::SphericParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, const glm::vec2& billboard_size,
	const Texture& texture, const glm::vec3& start_color, const glm::vec3& end_color, float radius, float particle_start_velocity, float gravity, float min_particle_lifetime_seconds, float max_particle_lifetime_seconds, float frequency)
	: ParticleEmitter(transformation, parent_transformable, "spheric_particle_emitter", billboard_size, true,
		static_cast<unsigned int>(ceil(frequency * max_particle_lifetime_seconds * 2.0F)) + 100
	),
	m_texture(texture),
	m_start_color(start_color),
	m_end_color(end_color),
	m_radius(radius),
	m_particle_start_velocity(particle_start_velocity),
	m_gravity(gravity),
	m_min_particle_lifetime_seconds(min_particle_lifetime_seconds),
	m_max_particle_lifetime_seconds(max_particle_lifetime_seconds) {
	ParticleEmitter::m_frequency = frequency;
}

void SphericParticleEmitter::draw(const Camera& camera) const {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	
	Drawable::m_render_program.set_uniform("u_texture", m_texture.get_id());
	Drawable::m_render_program.set_uniform("u_start_color", m_start_color);
	Drawable::m_render_program.set_uniform("u_end_color", m_end_color);
	m_texture.bind(GL_TEXTURE0);
	
	ParticleEmitter::draw(camera);
	
	Texture::unbind_any(GL_TEXTURE0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void SphericParticleEmitter::recalculate_properties() {
	glm::vec3 emission_direction = glm::normalize(glm::vec3(
		2.0F * (float) rand() / RAND_MAX - 1.0F,
		2.0F * (float) rand() / RAND_MAX - 1.0F,
		2.0F * (float) rand() / RAND_MAX - 1.0F
	));
	ParticleEmitter::m_particle_offset = glm::vec3(m_radius * emission_direction);
	ParticleEmitter::m_particle_start_velocity = m_particle_start_velocity * emission_direction;
	ParticleEmitter::m_particle_acceleration = m_gravity * glm::vec3(0.0F, -1.0F, 0.0F);
	ParticleEmitter::m_particle_lifetime_seconds = m_min_particle_lifetime_seconds + (float) rand() / RAND_MAX * (m_max_particle_lifetime_seconds - m_min_particle_lifetime_seconds);
}