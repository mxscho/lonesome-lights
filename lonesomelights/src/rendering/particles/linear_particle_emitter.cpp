#include "rendering/particles/linear_particle_emitter.h"

#include <cmath>
#include <cstdlib>
#include <string>
#include <GL/glew.h>

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/texture.h"

LinearParticleEmitter::LinearParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, const glm::vec2& billboard_size,
	const Texture& texture, const glm::vec3& start_color, const glm::vec3& end_color, float particle_velocity, unsigned int max_particle_count, float frequency)
	: ParticleEmitter(transformation, parent_transformable, "linear_particle_emitter", billboard_size, true, max_particle_count),
	m_texture(texture),
	m_start_color(start_color),
	m_end_color(end_color),
	m_particle_velocity(particle_velocity),
	m_target() {
	ParticleEmitter::m_particle_offset = glm::vec3(0.0F, 0.0F, 0.0F);
	ParticleEmitter::m_particle_acceleration = glm::vec3(0.0F);
	ParticleEmitter::m_frequency = frequency;
}

void LinearParticleEmitter::set_target(const glm::vec3& target) {
	m_target = target;
}

void LinearParticleEmitter::draw(const Camera& camera) const {
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	
	Drawable::m_render_program.set_uniform("u_texture", 0);
	Drawable::m_render_program.set_uniform("u_start_color", m_start_color);
	Drawable::m_render_program.set_uniform("u_end_color", m_end_color);
	m_texture.bind(GL_TEXTURE0);
	
	ParticleEmitter::draw(camera);
	
	Texture::unbind_any(GL_TEXTURE0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}
void LinearParticleEmitter::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	DeferredDrawable::m_deferred_render_program.set_uniform("u_start_color", m_start_color);
	DeferredDrawable::m_deferred_render_program.set_uniform("u_end_color", m_end_color);
	
	ParticleEmitter::draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
}

void LinearParticleEmitter::recalculate_properties() {
	glm::vec3 position_to_target = m_target - Transformable::get_position();
	ParticleEmitter::m_particle_start_velocity = m_particle_velocity * glm::normalize(position_to_target);
	ParticleEmitter::m_particle_lifetime_seconds = glm::length(position_to_target) / m_particle_velocity;
}