#include "game/player_handler.h"

#include <cmath>
#include <limits>
#include <iostream>

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
	// Unhover tiles.

	for (unsigned int i_y = 0; i_y < m_game.get_map().get_tile_count_y(); ++i_y) {
		for (unsigned int i_x = 0; i_x < m_game.get_map().get_tile_count_x(); ++i_x) {
			Tile& tile = m_game.get_map().get_tile(i_x, i_y);
			if (DestructibleRockTile* destructible_rock_tile = dynamic_cast<DestructibleRockTile*>(&tile)) {
				destructible_rock_tile->unhover();
			} else if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(&tile)) {
				crystal_tile->unhover();
			}
		}
	}

	// Unhover units.

	check_selected_unit();
	for (auto& i_own_unit : m_game.get_own_units()) {
		i_own_unit->unhover();
	}
	for (auto& i_opponent_unit : m_game.get_opponent_units()) {
		i_opponent_unit->unhover();
	}

	// Hover tiles.

	if (position.y >= 0.9F) {
		for (unsigned int i_y = 0; i_y < m_game.get_map().get_tile_count_y(); ++i_y) {
			for (unsigned int i_x = 0; i_x < m_game.get_map().get_tile_count_x(); ++i_x) {
				Tile& tile = m_game.get_map().get_tile(i_x, i_y);
				if (tile.get_x() == static_cast<unsigned int>(floor(position.x)) &&
					tile.get_y() == static_cast<unsigned int>(floor(position.z))) {
					if (DestructibleRockTile* destructible_rock_tile = dynamic_cast<DestructibleRockTile*>(&tile)) {
						if (destructible_rock_tile->is_selected()) {
							destructible_rock_tile->hover(glm::vec3(0.8F, 0.4F, 0.4F));
						} else {
							destructible_rock_tile->hover(glm::vec3(0.8F, 0.8F, 0.8F));
						}
					} else if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(&tile)) {
						if (crystal_tile->is_selected()) {
							crystal_tile->hover(glm::vec3(0.8F, 0.4F, 0.4F));
						} else {
							crystal_tile->hover(glm::vec3(0.8F, 0.8F, 0.8F));
						}
					}
				}
			}
		}
	}

	// Hover units.

	Unit* hovered_unit = nullptr;
	float hovered_unit_click_distance = std::numeric_limits<float>::max();
	bool is_own;
	for (auto& i_own_unit : m_game.get_own_units()) {
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
		// Select tile.

		if (position.y >= 0.9F) {
			for (unsigned int i_y = 0; i_y < m_game.get_map().get_tile_count_y(); ++i_y) {
				for (unsigned int i_x = 0; i_x < m_game.get_map().get_tile_count_x(); ++i_x) {
					Tile& tile = m_game.get_map().get_tile(i_x, i_y);
					if (tile.get_x() == static_cast<unsigned int>(floor(position.x)) &&
						tile.get_y() == static_cast<unsigned int>(floor(position.z))) {
						if (DestructibleRockTile* destructible_rock_tile = dynamic_cast<DestructibleRockTile*>(&tile)) {
							if (destructible_rock_tile->is_selected()) {	
								destructible_rock_tile->unselect();
								for (auto i_tile = m_game.m_own_selected_destructible_rock_tiles.begin(); i_tile != m_game.m_own_selected_destructible_rock_tiles.end(); ++i_tile) {
									if (*i_tile == destructible_rock_tile) {
										i_tile = m_game.m_own_selected_destructible_rock_tiles.erase(i_tile);
										return;
									}
								}
							} else {
								destructible_rock_tile->select(glm::vec3(1.0F, 1.0F, 1.0F));
								m_game.m_own_selected_destructible_rock_tiles.push_back(destructible_rock_tile);
							}
							return;
						} else if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(&tile)) {
							if (crystal_tile->is_selected()) {
								crystal_tile->unselect();
								for (auto i_tile = m_game.m_own_selected_crystal_tiles.begin(); i_tile != m_game.m_own_selected_crystal_tiles.end(); ++i_tile) {
									if (*i_tile == crystal_tile) {
										m_game.m_own_selected_crystal_tiles.erase(i_tile);
										break;
									}
								}
							} else {
								crystal_tile->select(glm::vec3(1.0F, 1.0F, 1.0F));
								m_game.m_own_selected_crystal_tiles.push_back(crystal_tile);
							}
							return;
						}
					}
				}
			}
		}

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
