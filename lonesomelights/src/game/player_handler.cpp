#include "game/player_handler.h"

#include <limits>

#include "game/units/laser_unit.h"

PlayerHandler::PlayerHandler(Game& game)
	: Updatable(),
	m_game(game),
	m_path_finder(m_game.get_map(), 12),
	m_selected_unit(nullptr) {
}

void PlayerHandler::update(const Timer& timer) {
	Updatable::update(timer);
}

void PlayerHandler::on_mouse_select(const Timer& timer, const glm::vec3& position, bool is_left, bool is_shift) {
	if (is_left) {
		// Select unit.
		
		Unit* selected_unit = nullptr;
		float selected_unit_click_distance = std::numeric_limits<float>::max();
		for (auto& i_own_unit : m_game.get_own_units()) {
			float click_distance = glm::length(i_own_unit->get_position() - position);
			if (click_distance < 0.35F &&
				(	
					selected_unit == nullptr ||
					click_distance < selected_unit_click_distance
				)
			) {
				selected_unit = &*i_own_unit;
				selected_unit_click_distance = click_distance;
			}
		}
		
		if (selected_unit != m_selected_unit) {
			if (m_selected_unit) {
				m_selected_unit->set_selected(false);
			}
			if (selected_unit) {
				selected_unit->set_selected(true);
			}
		}
		m_selected_unit = selected_unit;
	} else {
		// Move selected unit.
		
		if (m_selected_unit) {
			std::list<glm::vec2> path;
			if (is_shift) {
				path = m_path_finder.get_shortest_path(m_selected_unit->get_target_position_vec2(), glm::vec2(position.x, position.z), false);
				m_selected_unit->add_target_path_to_path(timer, path);
			} else {
				path = m_path_finder.get_shortest_path(m_selected_unit->get_position_vec2(), glm::vec2(position.x, position.z), false);
				m_selected_unit->set_target_path(timer, path);
			}
		}
	}
}