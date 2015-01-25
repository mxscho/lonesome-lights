#include "game/game.h"

#include <limits>
#include <set>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "game/attackable.h"
#include "game/map/base_tile.h"
#include "game/units/laser_unit.h"
#include "game/units/shockwave_unit.h"
#include "game/units/worker_unit.h"
#include "timer.h"

float Game::c_worker_unit_plasma_cost = 0.0F;
float Game::c_worker_unit_crystals_cost = 0.0F;
float Game::c_laser_unit_plasma_cost = 0.0F;
float Game::c_laser_unit_crystals_cost = 0.0F;
float Game::c_shockwave_unit_plasma_cost = 0.0F;
float Game::c_shockwave_unit_crystals_cost = 0.0F;

Game::Game()
	: Networkable(),
	Updatable(),
	m_map(Map::create_test_map(1.0F)), // TEST
	m_own_player(glm::vec3(0.0F, 0.0F, 0.8F)),
	m_opponent_player(glm::vec3(0.8F, 0.0F, 0.0F)),
	m_own_units(),
	m_opponent_units(),

	m_own_plasma(100.0F), // TEST
	m_own_crystals(100.0F),
	//m_opponent_plasma(100.0F),
	//m_opponent_crystals(100.0F),

	m_explosions() {

	m_map.set_tile(std::unique_ptr<Tile>(new BaseTile(BaseTile::create(m_map, 2, 6, m_own_player))));
	
	// TEST
	m_own_units.push_back(LaserUnit::create(glm::vec2(4.0F, 4.0F), m_map, m_own_player));
	m_own_units.push_back(LaserUnit::create(glm::vec2(2.0F, 2.0F), m_map, m_own_player));
	m_own_units.push_back(ShockwaveUnit::create(glm::vec2(8.0F, 2.0F), m_map, m_own_player));
	m_own_units.push_back(ShockwaveUnit::create(glm::vec2(1.0F, 1.0F), m_map, m_own_player));
	m_own_units.push_back(WorkerUnit::create(glm::vec2(2.0F, 15.0F), m_map, m_own_player));
	m_opponent_units.push_back(LaserUnit::create(glm::vec2(7.0F, 3.0F), m_map, m_opponent_player));
	m_opponent_units.push_back(ShockwaveUnit::create(glm::vec2(10.0F, 5.0F), m_map, m_opponent_player));
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
std::list<std::unique_ptr<Unit>>& Game::get_own_units() {
	return m_own_units;
}
std::list<std::unique_ptr<Unit>>& Game::get_opponent_units() {
	return m_opponent_units;
}

void Game::spawn_own_worker_unit() {
	if (m_own_plasma < c_worker_unit_plasma_cost || m_own_crystals < c_worker_unit_crystals_cost) {
		return;
	}
	m_own_units.push_back(WorkerUnit::create(glm::vec2(3.5F, 6.5F), m_map, m_own_player));
}
void Game::spawn_own_laser_unit() {
	if (m_own_plasma < c_laser_unit_plasma_cost || m_own_crystals < c_laser_unit_crystals_cost) {
		return;
	}
	m_own_units.push_back(LaserUnit::create(glm::vec2(3.5F, 6.5F), m_map, m_own_player));
}
void Game::spawn_own_shockwave_unit() {
	if (m_own_plasma < c_shockwave_unit_plasma_cost || m_own_crystals < c_shockwave_unit_crystals_cost) {
		return;
	}
	m_own_units.push_back(ShockwaveUnit::create(glm::vec2(3.5F, 6.5F), m_map, m_own_player));
}

void Game::draw(const Camera& camera) const {
	m_map.draw(camera);
	
	// Draw explosions.
	for (auto& i_explosion : m_explosions) {
		i_explosion.draw(camera);
	}
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
	
	m_map.draw_extras(camera);
}
void Game::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	for (auto& i_explosion : m_explosions) {
		i_explosion.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	}
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
	float delta_time_seconds = timer.get_delta_time_seconds();

	for (auto& i_own_unit : m_own_units) {
		if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			// Update attack relationships.
		
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
						attacked = i_opponent_unit.get();
						min_attack_range = attack_range;
					}
				}
				if (attacked) {
					own_laser_unit->start_shooting(attacked);
				}
			}
			
			// Update health.
			
			Attackable* attacked = own_laser_unit->get_shooting_target();
			if (attacked) {
				attacked->change_health(delta_time_seconds * -own_laser_unit->get_attack_dps());
			}
		}
		else if (ShockwaveUnit* own_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_own_unit.get())) {
			// Update attack relationships.
			
			float attack_range = own_shockwave_unit->get_attack_range();
			own_shockwave_unit->clear_attacks();
			for (auto& i_opponent_unit : m_opponent_units) {
				if (glm::distance(own_shockwave_unit->get_position(), i_opponent_unit->get_position()) < attack_range) {
					own_shockwave_unit->add_attack(i_opponent_unit.get());
				}
			}
			
			// Update health.
			
			for (auto& i_attackable : own_shockwave_unit->get_attacked()) {
				i_attackable->change_health(delta_time_seconds * -own_shockwave_unit->get_attack_dps());
			}
		}
	}	
	for (auto& i_opponent_unit : m_opponent_units) {
		if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			// Update attack relationships.
		
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
						attacked = i_own_unit.get();
						min_attack_range = attack_range;
					}
				}
				if (attacked) {
					opponent_laser_unit->start_shooting(attacked);
				}
			}
			
			// Update health.
			
			Attackable* attacked = opponent_laser_unit->get_shooting_target();
			if (attacked) {
				attacked->change_health(delta_time_seconds * -opponent_laser_unit->get_attack_dps());
			}
		}
		else if (ShockwaveUnit* opponent_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_opponent_unit.get())) {
			// Update attack relationships.
		
			float attack_range = opponent_shockwave_unit->get_attack_range();
			opponent_shockwave_unit->clear_attacks();
			for (auto& i_own_unit : m_own_units) {
				if (glm::distance(opponent_shockwave_unit->get_position(), i_own_unit->get_position()) < attack_range) {
					opponent_shockwave_unit->add_attack(i_own_unit.get());
				}
			}
			
			// Update health.
			
			for (auto& i_attackable : opponent_shockwave_unit->get_attacked()) {
				i_attackable->change_health(delta_time_seconds * -opponent_shockwave_unit->get_attack_dps());
			}
		}
	}
	
	// Update deaths.
	
	for (auto i_own_unit = m_own_units.begin(); i_own_unit != m_own_units.end(); ) {
		if ((*i_own_unit)->is_dead()) {
			for (auto& i_opponent_unit : m_opponent_units) {
				if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
					if (opponent_laser_unit->get_shooting_target() == i_own_unit->get()) {
						opponent_laser_unit->stop_shooting();
					}
				}
				else if (ShockwaveUnit* opponent_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_opponent_unit.get())) {
					opponent_shockwave_unit->remove_attack(i_own_unit->get());
				}
			}
			//glm::vec2 position = (*i_own_unit)->get_position_vec2();
			//m_explosions.emplace_back(glm::translate(glm::vec3(position.x, 0.2F, position.y)), m_map, 0.75F); // TODO: Fix move issues.
			//m_explosions.back().trigger(timer.get_current_time_seconds());
			i_own_unit = m_own_units.erase(i_own_unit);
		} else {
			++i_own_unit;
		}
	}
	for (auto i_opponent_unit = m_opponent_units.begin(); i_opponent_unit != m_opponent_units.end(); ) {
		if ((*i_opponent_unit)->is_dead()) {
			for (auto& i_own_unit : m_own_units) {
				if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
					if (own_laser_unit->get_shooting_target() == i_opponent_unit->get()) {
						own_laser_unit->stop_shooting();
					}
				}
				else if (ShockwaveUnit* own_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_own_unit.get())) {
					own_shockwave_unit->remove_attack(i_opponent_unit->get());
				}
			}
			//glm::vec2 position = (*i_opponent_unit)->get_position_vec2();
			//m_explosions.emplace_back(glm::translate(glm::vec3(position.x, 0.2F, position.y)), m_map, 0.75F); // TODO: Fix move issues.
			//m_explosions.back().trigger(timer.get_current_time_seconds());
			i_opponent_unit = m_opponent_units.erase(i_opponent_unit);
		} else {
			++i_opponent_unit;
		}
	}
	
	// Update units.
	
	for (auto& i_own_unit : m_own_units) {
		i_own_unit->update(timer);
	}
	for (auto& i_opponent_unit : m_opponent_units) {
		i_opponent_unit->update(timer);
	}
	
	// Update explosions.
	
	for (auto i_explosion = m_explosions.begin(); i_explosion != m_explosions.end(); ) {
		if (i_explosion->has_finished()) {
			i_explosion = m_explosions.erase(i_explosion);
		} else {
			i_explosion->update(timer);
			++i_explosion;
		}
	}
}
