#include "rendering/particles/particle_systems/laser.h"

#include "rendering/opengl/textures.h"
#include <iostream>

Laser::Laser(const glm::mat4& transformation, const Transformable& parent_transformable, const glm::vec3& color)
	: Updatable(),
	Transformable(transformation, parent_transformable),
	m_is_enabled(false),
	m_target(),
	m_laser(
		transformation, // Transformation
		parent_transformable, // Transformable
		glm::vec2(3.0F * 0.15F, 0.15F), // Billboard size
		Textures::get_texture("particles/laser"), // Texture
		color, // Start color
		color, // End color
		20.0F, // Particle velocity
		static_cast<unsigned int>(ceil(
			100.0F * // Frequency
			(10.0F / // Distance
			20.0F) * // Particle velocity
			1.1F)) + 10, // Maximum particle count (should fit for about 10 range laser)
		100.0F  // Frequency
	), m_sparks(
		transformation, // Transformation
		parent_transformable, // Parent transformable
		glm::vec2(12.0F * 0.03F, 0.03F), // Billboard size
		Textures::get_texture("particles/spark"), // Texture
		color, // Start color
		glm::vec3(1.0F, 1.0F, 1.0F), // End color
		0.0F, // Radius
		1.0F, // Particle start velocity
		0.0F, // Gravity
		0.5F, // Minimum particle lifetime (seconds)
		0.25F, // Maximum particle lifetime (seconds)
		25.0F  // Frequency
	) {
	m_laser.set_emitting(false);
	m_sparks.set_emitting(false);
}

void Laser::set_target(const glm::vec3& target) {
	m_target = target;
}
void Laser::set_enabled(bool is_enabled) {
	m_is_enabled = is_enabled;
	m_laser.set_emitting(is_enabled);
	m_sparks.set_emitting(is_enabled);
}

void Laser::draw(const Camera& camera) const {
	if (!m_is_enabled) {
		return;
	}

	m_laser.draw(camera);
	m_sparks.draw(camera);
}
void Laser::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	if (!m_is_enabled) {
		return;
	}

	m_laser.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	m_sparks.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
}

void Laser::update(const Timer& timer) {
	if (!m_is_enabled) {
		return;
	}
	m_laser.Transformable::set_position(Transformable::get_position());
	m_laser.set_target(m_target);
	m_sparks.Transformable::set_position(m_target);
	
	m_laser.update(timer);
	m_sparks.update(timer);
}