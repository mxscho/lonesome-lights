#include "rendering/particles/particle_systems/explosion.h"

#include "timer.h"
#include "rendering/opengl/textures.h"

const float Explosion::c_end_time_seconds = 1.2F;
const float Explosion::c_smoke_trails_start_time_seconds = 0.4F;
const float Explosion::c_smoke_trails_end_time_seconds = 1.2F;
const float Explosion::c_sparks_start_time_seconds = 0.4F;
const float Explosion::c_sparks_end_time_seconds = 1.0F;
const float Explosion::c_flames_start_time_seconds = 0.1F;
const float Explosion::c_flames_end_time_seconds = 1.1F;
const float Explosion::c_flash_start_time_seconds = 0.1F;
const float Explosion::c_flash_end_time_seconds = 0.9F;
const float Explosion::c_smoke_start_time_seconds = 1.3F;
const float Explosion::c_smoke_end_time_seconds = 1.5F;

Explosion::Explosion(const glm::mat4& transformation, const Transformable& parent_transformable, float intensity)
	: Updatable(),
	Transformable(transformation, parent_transformable),
	m_has_finished(true),
	m_start_time_seconds(),
	m_max_particle_lifetime_seconds(2.75F),
	m_smoke_trails(
		glm::mat4(), // Transformation
		*this, // Parent transformable
		glm::vec2(intensity * 4.6F * 0.2F, intensity * 0.2F), // Billboard size
		Textures::get_texture("particles/smoke_trail"), // Texture
		glm::vec3(1.0F, 0.8F, 0.0F), // Start color
		glm::vec3(1.0F, 0.7F, 0.0F), // End color
		0.0F, // Radius
		intensity * 1.0F, // Particle start velocity
		0.1F, // Gravity
		1.0F, // Minimum particle lifetime (seconds)
		1.5F, // Maximum particle lifetime (seconds)
		intensity * 15.0F  // Frequency
	), m_sparks(
		glm::mat4(), // Transformation
		*this, // Parent transformable
		glm::vec2(intensity * 12.0F * 0.05F, intensity * 0.05F), // Billboard size
		Textures::get_texture("particles/spark"), // Texture
		glm::vec3(1.0F, 0.7F, 0.0F), // Start color
		glm::vec3(1.0F, 1.0F, 1.0F), // End color
		0.0F, // Radius
		intensity * 1.5F, // Particle start velocity
		0.0F, // Gravity
		1.5F, // Minimum particle lifetime (seconds)
		2.0F, // Maximum particle lifetime (seconds)
		intensity * 25.0F  // Frequency
	), m_flames(
		glm::mat4(), // Transformation
		*this, // Parent transformable
		glm::vec2(intensity * 0.8F, intensity * 0.8F), // Billboard size
		Textures::get_texture("particles/smoke"), // Texture
		glm::vec3(1.0F, 0.7F, 0.1F), // Start color
		glm::vec3(1.0F, 0.2F, 0.1F), // End color
		0.0F, // Radius
		intensity * 0.2F, // Particle start velocity
		0.0F, // Gravity
		2.0F, // Minimum particle lifetime (seconds)
		2.5F, // Maximum particle lifetime (seconds)
		intensity * 15.0F  // Frequency
	), m_flash(
		glm::mat4(), // Transformation
		*this, // Parent transformable
		glm::vec2(intensity * 1.0F, intensity * 1.0F), // Billboard size
		Textures::get_texture("particles/flash"), // Texture
		glm::vec3(1.0F, 0.8F, 0.5F), // Start color
		glm::vec3(1.0F, 1.0F, 1.0F), // End color
		0.0F, // Radius
		intensity * 0.1F, // Particle start velocity
		0.0F, // Gravity
		0.25F, // Minimum particle lifetime (seconds)
		0.75F, // Maximum particle lifetime (seconds)
		intensity * 5.0F  // Frequency
	), m_smoke(
		glm::mat4(), // Transformation
		*this, // Parent transformable
		glm::vec2(intensity * 1.0F, intensity * 1.0F), // Billboard size
		Textures::get_texture("particles/smoke"), // Texture
		glm::vec3(0.1F, 0.1F, 0.1F), // Start color
		glm::vec3(0.3F, 0.3F, 0.3F), // End color
		0.0F, // Radius
		intensity * 0.15F, // Particle start velocity
		0.0F, // Gravity
		2.0F, // Minimum particle lifetime (seconds)
		2.5F, // Maximum particle lifetime (seconds)
		intensity * 50.0F  // Frequency
	) {
	m_smoke_trails.set_emitting(false);
	m_sparks.set_emitting(false);
	m_flames.set_emitting(false);
	m_flash.set_emitting(false);
	m_smoke.set_emitting(false);
}

void Explosion::trigger(float current_time_seconds) {	
	m_start_time_seconds = current_time_seconds;
	m_has_finished = false;
}
bool Explosion::has_finished() const {
	return m_has_finished;
}

void Explosion::draw(const Camera& camera) const {
	if (m_has_finished) {
		return;
	}

	m_smoke_trails.draw(camera);
	m_sparks.draw(camera);
	m_flames.draw(camera);
	m_flash.draw(camera);
	m_smoke.draw(camera);
}
void Explosion::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	if (m_has_finished) {
		return;
	}

	m_smoke_trails.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	m_sparks.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	m_flames.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	m_flash.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	m_smoke.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
}

void Explosion::update(const Timer& timer) {
	if (m_has_finished) {
		return;
	}
	float local_time_seconds = timer.get_current_time_seconds() - m_start_time_seconds;
	if (local_time_seconds >= Explosion::c_end_time_seconds + m_max_particle_lifetime_seconds) {
		m_has_finished = true;
		
		m_smoke_trails.set_emitting(false);
		m_sparks.set_emitting(false);
		m_flames.set_emitting(false);
		m_flash.set_emitting(false);
		m_smoke.set_emitting(false);
		return;
	}
	
	if (local_time_seconds >= c_smoke_trails_end_time_seconds) {
		m_smoke_trails.set_emitting(false);
	} else if (local_time_seconds >= c_smoke_trails_start_time_seconds) {
		m_smoke_trails.set_emitting(true);
	}
	if (local_time_seconds >= c_sparks_end_time_seconds) {
		m_sparks.set_emitting(false);
	} else if (local_time_seconds >= c_sparks_start_time_seconds) {
		m_sparks.set_emitting(true);
	}
	if (local_time_seconds >= c_flames_end_time_seconds) {
		m_flames.set_emitting(false);
	} else if (local_time_seconds >= c_flames_start_time_seconds) {
		m_flames.set_emitting(true);
	}
	if (local_time_seconds >= c_flash_end_time_seconds) {
		m_flash.set_emitting(false);
	} else if (local_time_seconds >= c_flash_start_time_seconds) {
		m_flash.set_emitting(true);
	}
	if (local_time_seconds >= c_smoke_end_time_seconds) {
		m_smoke.set_emitting(false);
	} else if (local_time_seconds >= c_smoke_start_time_seconds) {
		m_smoke.set_emitting(true);
	}
	
	m_smoke_trails.update(timer);
	m_sparks.update(timer);
	m_flames.update(timer);
	m_flash.update(timer);
	m_smoke.update(timer);
}