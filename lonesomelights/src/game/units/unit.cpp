#include "game/units/unit.h"

#include "game/map/map.h"
#include "rendering/opengl/texture.h"
#include "rendering/opengl/textures.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

Unit::Unit(const glm::mat4& transformation, const glm::vec2& position, const Map& map, const Player& player, float max_velocity, float acceleration, float decceleration, float max_health)
	: Attackable(glm::inverse(transformation) * glm::translate(glm::vec3(0.0F, 0.90F, 0.0F)), *this, max_health, 75.0F),
	Networkable(),
	InertialMovable(glm::translate(glm::vec3(position.x, 0.0F, position.y)) * transformation, map, max_velocity, acceleration, decceleration),
	m_player(player),
	m_is_selected(false),
	m_is_hovered(false),
	m_selection_circle(glm::translate(glm::vec3(position.x, 0.01F, position.y)) * glm::scale(glm::vec3(0.5F, 1.0F, 0.5F)), map) {
}

glm::vec2 Unit::get_position_vec2() const {
	glm::vec3 vec3 = InertialMovable::get_position();
	return glm::vec2(vec3.x, vec3.z);
}
bool Unit::is_selected() const {
	return m_is_selected;
}
void Unit::unselect() {
	m_is_selected = false;
}
void Unit::select(const glm::vec3& color) {
	m_is_selected = true;
	m_selection_circle.set_color(color);
}
void Unit::unhover() {
	m_is_hovered = false;
}
void Unit::hover(const glm::vec3& color) {
	if (m_is_selected || m_is_hovered) {
		return;
	}
	m_is_hovered = true;
	m_selection_circle.set_color(color);
}

glm::vec2 Unit::get_target_position_vec2() const {
	glm::vec3 target_position = InertialMovable::get_target_position();
	return glm::vec2(target_position.x, target_position.z);
}
void Unit::set_target_path(const Timer& timer, const std::list<glm::vec2>& target_path) {
	if (target_path.size() == 0) {
		return;
	}

	std::list<glm::vec3> target_path_vec3;
	for (auto& i_target_position : target_path) {
		target_path_vec3.push_back(glm::vec3(i_target_position.x, InertialMovable::get_position().y, i_target_position.y));
	}
	InertialMovable::set_target_path(timer, target_path_vec3);
}
void Unit::set_target_position(const Timer& timer, const glm::vec2& target_position) {
	InertialMovable::set_target_position(timer, glm::vec3(target_position.x, InertialMovable::get_position().y, target_position.y));
}
void Unit::add_target_position_to_path(const Timer& timer, const glm::vec2& target_position) {
	InertialMovable::add_target_position_to_path(timer, glm::vec3(target_position.x, InertialMovable::get_position().y, target_position.y));
}
void Unit::add_target_path_to_path(const Timer& timer, const std::list<glm::vec2>& target_path) {
	if (target_path.size() == 0) {
		return;
	}

	std::list<glm::vec3> target_path_vec3;
	for (auto& i_target_position : target_path) {
		target_path_vec3.push_back(glm::vec3(i_target_position.x, InertialMovable::get_position().y, i_target_position.y));
	}
	InertialMovable::add_target_path_to_path(timer, target_path_vec3);
}

void Unit::draw(const Camera& camera) const {
	Attackable::draw(camera);

	if (m_is_selected || m_is_hovered) {
		m_selection_circle.draw(camera);
	}
}

void Unit::update(const Timer& timer) {
	InertialMovable::update(timer);
	
	glm::vec3 unit_position = InertialMovable::get_position();
	m_selection_circle.set_position(glm::vec3(unit_position.x, 0.01F, unit_position.z));
	
	Attackable::update(timer);
}