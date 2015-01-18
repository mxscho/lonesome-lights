#include "game/units/unit.h"

#include "game/map/map.h"
#include "rendering/opengl/render_programs.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

Unit::Unit(const glm::mat4& transformation, const glm::vec2& position, const Map& map, const Player& player, float max_velocity, float acceleration, float decceleration)
	: Drawable(RenderPrograms::get_render_program("unit")),
	Networkable(),
	InertialMovable(glm::translate(glm::vec3(position.x, 0.1F, position.y)) * transformation, map, max_velocity, acceleration, decceleration),
	m_player(player) {
}

glm::vec2 Unit::get_position_vec2() const {
	glm::vec3 vec3 = InertialMovable::get_position();
	return glm::vec2(vec3.x, vec3.z);
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

void Unit::update(const Timer& timer) {
	InertialMovable::update(timer);
}