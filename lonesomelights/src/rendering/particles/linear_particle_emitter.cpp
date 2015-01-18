#include "rendering/particles/linear_particle_emitter.h"

#include <cmath>
#include <cstdlib>
#include <string>
#include <GL/glew.h>

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/texture.h"

LinearParticleEmitter::LinearParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, const glm::vec2& billboard_size,
	const Texture& texture, const glm::vec3& start_color, const glm::vec3& end_color, const glm::vec3& particle_start_velocity, float particle_offset, float max_particle_distance, float frequency)
	: ParticleEmitter(transformation, parent_transformable, "linear_particle_emitter", billboard_size, true,
		static_cast<unsigned int>(ceil(max_particle_distance / glm::length(particle_start_velocity) * frequency * 2.0F)) + 10
	),
	m_texture(texture),
	m_start_color(start_color),
	m_end_color(end_color),
	m_max_particle_distance(max_particle_distance) {
	ParticleEmitter::m_particle_offset = particle_offset * glm::normalize(particle_start_velocity);
	ParticleEmitter::m_particle_start_velocity = particle_start_velocity;
	ParticleEmitter::m_particle_acceleration = glm::vec3(0.0F);
	ParticleEmitter::m_frequency = frequency;
	ParticleEmitter::m_particle_lifetime_seconds = max_particle_distance / glm::length(particle_start_velocity);
}

void LinearParticleEmitter::draw(const Camera& camera) const {
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

void LinearParticleEmitter::recalculate_properties() {
}