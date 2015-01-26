#include "game/attackable.h"

#include <algorithm>

Attackable::Attackable(const glm::mat4& transformation, const Transformable& parent_transformable, float max_health, float health)
	: m_max_health(max_health),
	m_health_bar(transformation, parent_transformable, max_health, health) {
}

void Attackable::change_health(float delta_health) {
	float health = std::max(0.0F, m_health_bar.get_health() + delta_health);
	m_health_bar.set_health(health);
}
bool Attackable::is_dead() const {
	return m_health_bar.get_health() == 0;
}
bool Attackable::is_full() const {
	return m_health_bar.get_health() == m_max_health;
}

void Attackable::draw(const Camera& camera) const {
	if (is_full()) {
		return;
	}

	m_health_bar.draw(camera);
}

void Attackable::update(const Timer& timer) {
	m_health_bar.update(timer);
}
