#include "game/attackable.h"

Attackable::Attackable(const glm::mat4& transformation, const Transformable& parent_transformable, float max_health, float health)
	: m_health_bar(transformation, parent_transformable, max_health, health) {
}

void Attackable::draw(const Camera& camera) const {
	m_health_bar.draw(camera);
}

void Attackable::update(const Timer& timer) {
	m_health_bar.update(timer);
}