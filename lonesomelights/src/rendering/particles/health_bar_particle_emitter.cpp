#include "rendering/particles/health_bar_particle_emitter.h"

#include <cmath>
#include <cstdlib>
#include <string>
#include <GL/glew.h>

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/texture.h"
#include "rendering/opengl/textures.h"

HealthBarParticleEmitter::HealthBarParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, float max_health, float health)
	: ParticleEmitter(transformation, parent_transformable, "health_bar_particle_emitter", glm::vec2(0.5F, 0.075F), false, true,
		2
	),
	m_max_health(max_health),
	m_health(health) {
	ParticleEmitter::m_particle_offset = glm::vec3(0.0F);
	ParticleEmitter::m_particle_start_velocity = glm::vec3(0.0F);
	ParticleEmitter::m_particle_acceleration = glm::vec3(0.0F);
	ParticleEmitter::m_particle_lifetime_seconds = 1000.0F * 365.25F * 24.0F * 3600.0F;
	ParticleEmitter::m_frequency = 1 / m_particle_lifetime_seconds;
}

float HealthBarParticleEmitter::get_health() const {
	return m_health;
}
void HealthBarParticleEmitter::set_health(float health) {
	m_health = health;
}

void HealthBarParticleEmitter::draw(const Camera& camera) const {
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	
	const Texture& texture = Textures::get_texture("particles/health_bar");
	Drawable::m_render_program.set_uniform("u_texture", 0);
	Drawable::m_render_program.set_uniform("u_max_health", m_max_health);
	Drawable::m_render_program.set_uniform("u_health", m_health);
	
	texture.bind(GL_TEXTURE0);
	
	ParticleEmitter::draw(camera);
	
	Texture::unbind_any(GL_TEXTURE0);
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}
void HealthBarParticleEmitter::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
}

void HealthBarParticleEmitter::recalculate_properties() {
}
