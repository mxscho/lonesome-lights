#include "rendering/particles/particle_systems/smoke.h"

#include "timer.h"
#include "rendering/opengl/textures.h"

const float Smoke::c_end_time_seconds = 0.0F;
const float Smoke::c_smoke_start_time_seconds = 0.1F;
const float Smoke::c_smoke_end_time_seconds = 0.3F;

Smoke::Smoke(const glm::mat4& transformation, const Transformable& parent_transformable, float intensity)
	: Updatable(),
	Transformable(transformation, parent_transformable),
	m_has_finished(true),
	m_start_time_seconds(),
	m_max_particle_lifetime_seconds(2.75F),
	m_smoke(
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

	m_smoke.set_emitting(false);
}

void Smoke::trigger(float current_time_seconds) {	
	m_start_time_seconds = current_time_seconds;
	m_has_finished = false;
}
bool Smoke::has_finished() const {
	return m_has_finished;
}

void Smoke::draw(const Camera& camera) const {
	if (m_has_finished) {
		return;
	}
	m_smoke.draw(camera);
}
void Smoke::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	if (m_has_finished) {
		return;
	}

	m_smoke.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
}

void Smoke::update(const Timer& timer) {
	if (m_has_finished) {
		return;
	}
	float local_time_seconds = timer.get_current_time_seconds() - m_start_time_seconds;
	if (local_time_seconds >= Smoke::c_end_time_seconds + m_max_particle_lifetime_seconds) {
		m_has_finished = true;

		m_smoke.set_emitting(false);
		return;
	}
	
	if (local_time_seconds >= c_smoke_end_time_seconds) {
		m_smoke.set_emitting(false);
	} else if (local_time_seconds >= c_smoke_start_time_seconds) {
		m_smoke.set_emitting(true);
	}
	
	m_smoke.update(timer);
}
