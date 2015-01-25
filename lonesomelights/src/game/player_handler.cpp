#include "game/player_handler.h"

#include <limits>

#include "game/units/laser_unit.h"

PlayerHandler::PlayerHandler(Game& game)
	: Updatable(),
	m_game(game),
	m_path_finder(m_game.get_map(), 4),
	m_selected_unit(nullptr) {
}

void PlayerHandler::update(const Timer& timer) {
	Updatable::update(timer);
}

void PlayerHandler::on_mouse_hover(const Timer& timer, const glm::vec3& position) {
	check_selected_unit();

	Unit* hovered_unit = nullptr;
	float hovered_unit_click_distance = std::numeric_limits<float>::max();
	bool is_own;
	for (auto& i_own_unit : m_game.get_own_units()) {
		i_own_unit->unhover();
		float click_distance = glm::length(i_own_unit->get_position() - position);
		if (click_distance < 0.35F &&
			(	
				hovered_unit == nullptr ||
				click_distance < hovered_unit_click_distance
			)
		) {
			hovered_unit = &*i_own_unit;
			hovered_unit_click_distance = click_distance;
			is_own = true;
		}
	}
	for (auto& i_opponent_unit : m_game.get_opponent_units()) {
		i_opponent_unit->unhover();
		float click_distance = glm::length(i_opponent_unit->get_position() - position);
		if (click_distance < 0.35F &&
			(	
				hovered_unit == nullptr ||
				click_distance < hovered_unit_click_distance
			)
		) {
			hovered_unit = &*i_opponent_unit;
			hovered_unit_click_distance = click_distance;
			is_own = false;
		}
	}
	
	if (hovered_unit) {
		if (is_own) {
			hovered_unit->hover(glm::vec3(0.8F, 0.8F, 0.8F));
		} else if (m_selected_unit) {
			hovered_unit->hover(glm::vec3(0.8F, 0.4F, 0.4F));
		}
	}
}
void PlayerHandler::on_mouse_select(const Timer& timer, const glm::vec3& position, bool is_left, bool is_shift) {
	check_selected_unit();

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
				m_selected_unit->unselect();
			}
			if (selected_unit) {
				selected_unit->select(glm::vec3(1.0F, 1.0F, 1.0F));
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

void PlayerHandler::check_selected_unit() {
	bool existing = false;
	for (auto& i_own_unit : m_game.get_own_units()) {
		if (i_own_unit.get() == m_selected_unit) {
			existing = true;
		}
	}
	if (!existing) {
		m_selected_unit = nullptr;
	}
}
