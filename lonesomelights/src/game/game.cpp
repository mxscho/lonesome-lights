#include "game/game.h"

#include <glm/glm.hpp>
#include <limits>

#include "game/attackable.h"
#include "game/units/laser_unit.h"
#include "game/units/shockwave_unit.h"
#include "game/units/worker_unit.h"


Game::Game()
	: Networkable(),
	Updatable(),
	m_map(Map::create_test_map(1.0F)), // TEST
	m_own_player(glm::vec3(0.0F, 0.0F, 0.8F)),
	m_opponent_player(glm::vec3(0.8F, 0.0F, 0.0F)),
	m_own_units(),
	m_opponent_units() {
	
	// TEST
	m_own_units.push_back(LaserUnit::create(glm::vec2(4.0F, 4.0F), m_map, m_own_player));
	m_own_units.push_back(LaserUnit::create(glm::vec2(2.0F, 2.0F), m_map, m_own_player));
	m_own_units.push_back(ShockwaveUnit::create(glm::vec2(8.0F, 2.0F), m_map, m_own_player));
	m_own_units.push_back(WorkerUnit::create(glm::vec2(2.0F, 15.0F), m_map, m_own_player));
	m_opponent_units.push_back(LaserUnit::create(glm::vec2(7.0F, 3.0F), m_map, m_opponent_player));
	m_opponent_units.push_back(LaserUnit::create(glm::vec2(15.0F, 3.0F), m_map, m_opponent_player));
	
	// -- for test map' base ---
	Tile& base_up = m_map.get_tile(2, 7);
	Tile& base_down = m_map.get_tile(2, 5);
	Tile& base_left = m_map.get_tile(1, 6);
	Tile& base_right = m_map.get_tile(3, 6);
	base_up.set_is_walkable(false);
	base_down.set_is_walkable(false);
	base_left.set_is_walkable(false);
	base_right.set_is_walkable(false);
	// --------------------
}

Map& Game::get_map() {
	return m_map;
}
std::vector<std::unique_ptr<Unit>>& Game::get_own_units() {
	return m_own_units;
}
std::vector<std::unique_ptr<Unit>>& Game::get_opponent_units() {
	return m_opponent_units;
}

void Game::draw(const Camera& camera) const {
	m_map.draw(camera);
	
	// Draw own weapon particles.
	for (auto& i_own_unit : m_own_units) {
		if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			own_laser_unit->draw_laser(camera);
		}
	}
	// Draw opponent's weapon particles.
	for (auto& i_opponent_unit : m_opponent_units) {
		if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			opponent_laser_unit->draw_laser(camera);
		}
	}
	// Draw own units.
	for (auto& i_own_unit : m_own_units) {
		if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			own_laser_unit->draw(camera);
		}
		if (ShockwaveUnit* own_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_own_unit.get())) {
			own_shockwave_unit->draw(camera);
		}
		if (WorkerUnit* own_worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
			own_worker_unit->draw(camera);
		}
	}
	// Draw opponent's units.
	for (auto& i_opponent_unit : m_opponent_units) {
		if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			opponent_laser_unit->draw(camera);
		}
		if (ShockwaveUnit* opponent_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_opponent_unit.get())) {
			opponent_shockwave_unit->draw(camera);
		}
		if (WorkerUnit* opponent_worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
			opponent_worker_unit->draw(camera);
		}
	}
}
void Game::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	for (auto& i_own_unit : m_own_units) {
		if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			own_laser_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		}
	}
	for (auto& i_opponent_unit : m_opponent_units) {
		if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			opponent_laser_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		}
	}
}

void Game::update(const Timer& timer) {
	// Update own units.
	
	for (auto& i_own_unit : m_own_units) {
		if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			float attack_range = own_laser_unit->get_attack_range();
			Attackable* currently_attacked = own_laser_unit->get_shooting_target();
			if (currently_attacked) {
				if (glm::distance(own_laser_unit->get_position_vec2(), currently_attacked->get_position_vec2()) > attack_range) {
					own_laser_unit->stop_shooting();
					currently_attacked = nullptr;
				}
			}
			if (!currently_attacked) {		
				Attackable* attacked = nullptr;
				float min_attack_range = std::numeric_limits<float>::max();
				for (auto& i_opponent_unit : m_opponent_units) {
					if (glm::distance(own_laser_unit->get_position(), i_opponent_unit->get_position()) < attack_range &&
						attack_range < min_attack_range) {
						attacked = &*i_opponent_unit;
						min_attack_range = attack_range;
					}
				}
				if (attacked) {
					own_laser_unit->start_shooting(attacked);
				}
			}
		}
		
		i_own_unit->update(timer);
	}
	
	// Update opponent units.
	
	for (auto& i_opponent_unit : m_opponent_units) {
		if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			float attack_range = opponent_laser_unit->get_attack_range();
			Attackable* currently_attacked = opponent_laser_unit->get_shooting_target();
			if (currently_attacked) {
				if (glm::distance(opponent_laser_unit->get_position_vec2(), currently_attacked->get_position_vec2()) > attack_range) {
					opponent_laser_unit->stop_shooting();
					currently_attacked = nullptr;
				}
			}
			if (!currently_attacked) {		
				Attackable* attacked = nullptr;
				float min_attack_range = std::numeric_limits<float>::max();
				for (auto& i_own_unit : m_own_units) {
					if (glm::distance(opponent_laser_unit->get_position(), i_own_unit->get_position()) < attack_range &&
						attack_range < min_attack_range) {
						attacked = &*i_own_unit;
						min_attack_range = attack_range;
					}
				}
				if (attacked) {
					opponent_laser_unit->start_shooting(attacked);
				}
			}
		}
		
		i_opponent_unit->update(timer);
	}
}
