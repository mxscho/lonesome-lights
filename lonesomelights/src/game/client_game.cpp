#include "game/client_game.h"

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

#include <iostream>

float ClientGame::c_worker_unit_plasma_cost = 100.0F;
float ClientGame::c_worker_unit_crystals_cost = 20.0F;
float ClientGame::c_laser_unit_plasma_cost = 100.0F;
float ClientGame::c_laser_unit_crystals_cost = 20.0F;
float ClientGame::c_shockwave_unit_plasma_cost = 100.0F;
float ClientGame::c_shockwave_unit_crystals_cost = 20.0F;

unsigned int ClientGame::c_own_base_x = 27;
unsigned int ClientGame::c_own_base_y = 2;
unsigned int ClientGame::c_opponent_base_x = 2;
unsigned int ClientGame::c_opponent_base_y = 27;

ClientGame::ClientGame(Client& client)
	: Networkable(),
	Updatable(),

	m_own_selected_destructible_rock_tiles(),
	m_own_selected_crystal_tiles(),
	m_opponent_selected_destructible_rock_tiles(),
	m_opponent_selected_crystal_tiles(),

	m_is_started(false),

	m_map(Map::create_test_map(1.0F)), // TEST
	m_own_player(glm::vec3(0.0F, 0.0F, 0.8F)),
	m_opponent_player(glm::vec3(0.8F, 0.0F, 0.0F)),
	m_own_units(),
	m_opponent_units(),

	m_own_plasma_count(1000.0F), // TEST
	m_own_crystal_count(100.0F),
	m_opponent_plasma_count(1000.0F),
	m_opponent_crystal_count(100.0F),

	m_explosions(),
	m_client(client) {

	m_map.set_tile(std::unique_ptr<Tile>(new BaseTile(BaseTile::create(m_map, c_own_base_x, c_own_base_y, m_own_player))));
	m_map.set_tile(std::unique_ptr<Tile>(new BaseTile(BaseTile::create(m_map, c_opponent_base_x, c_opponent_base_y, m_opponent_player))));

	// TEST
	m_own_units.push_back(LaserUnit::create(glm::vec2(4.0F, 4.0F), m_map, m_own_player));
	m_own_units.push_back(LaserUnit::create(glm::vec2(2.0F, 2.0F), m_map, m_own_player));
	m_own_units.push_back(ShockwaveUnit::create(glm::vec2(8.0F, 2.0F), m_map, m_own_player));
	m_own_units.push_back(ShockwaveUnit::create(glm::vec2(1.0F, 1.0F), m_map, m_own_player));
	m_own_units.push_back(WorkerUnit::create(glm::vec2(2.0F, 15.0F), m_map, m_own_player));
	//static_cast<WorkerUnit*>(m_own_units.back().get())->start_exploiting(static_cast<CrystalTile*>(&m_map.get_tile(9, 9)));
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

bool ClientGame::has_started() const {
	return m_is_started;
}
void ClientGame::start() {
	m_is_started = true;
}

float ClientGame::get_own_plasma_count() const {
	return m_own_plasma_count;
}
float ClientGame::get_own_crystal_count() const {
	return m_own_crystal_count;
}

Map& ClientGame::get_map() {
	return m_map;
}
std::list<std::unique_ptr<Unit>>& ClientGame::get_own_units() {
	return m_own_units;
}
std::list<std::unique_ptr<Unit>>& ClientGame::get_opponent_units() {
	return m_opponent_units;
}

void ClientGame::spawn_own_worker_unit() {
	// CLIENT SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << 0;
	m_client.get_participant().add_outgoing_network_packet(network_packet);
}
void ClientGame::spawn_own_laser_unit() {
	// CLIENT SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << 1;
	m_client.get_participant().add_outgoing_network_packet(network_packet);
}
void ClientGame::spawn_own_shockwave_unit() {
	// CLIENT SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << 2;
	m_client.get_participant().add_outgoing_network_packet(network_packet);
}

void ClientGame::draw(const Camera& camera) const {
	m_map.draw(camera);
	
	// Draw explosions.
	for (auto& i_explosion : m_explosions) {
		i_explosion.draw(camera);
	}
	// Draw own weapon particles.
	for (auto& i_own_unit : m_own_units) {
		if (WorkerUnit* own_worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
			own_worker_unit->draw_laser(camera);
		} else if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			own_laser_unit->draw_laser(camera);
		}
	}
	// Draw opponent's weapon particles.
	for (auto& i_opponent_unit : m_opponent_units) {
		if (WorkerUnit* opponent_worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
			opponent_worker_unit->draw_laser(camera);
		} else if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			opponent_laser_unit->draw_laser(camera);
		}
	}
	
	// Draw own units' selection circles.
	for (auto& i_own_unit : m_own_units) {
		i_own_unit->draw_selection_circle(camera);
	}
	// Draw opponent's units' selection circles.
	for (auto& i_opponent_unit : m_opponent_units) {
		i_opponent_unit->draw_selection_circle(camera);
	}
	
	// Draw own units.
	for (auto& i_own_unit : m_own_units) {
		if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			own_laser_unit->draw(camera);
		} else if (ShockwaveUnit* own_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_own_unit.get())) {
			own_shockwave_unit->draw(camera);
		} else if (WorkerUnit* own_worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
			own_worker_unit->draw(camera);
		}
	}
	// Draw opponent's units.
	for (auto& i_opponent_unit : m_opponent_units) {
		if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			opponent_laser_unit->draw(camera);
		} else if (ShockwaveUnit* opponent_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_opponent_unit.get())) {
			opponent_shockwave_unit->draw(camera);
		} else if (WorkerUnit* opponent_worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
			opponent_worker_unit->draw(camera);
		}
	}
	
	m_map.draw_extras(camera);
}
void ClientGame::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	for (auto& i_explosion : m_explosions) {
		i_explosion.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	}
	for (auto& i_own_unit : m_own_units) {
		if (WorkerUnit* own_worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
			own_worker_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		} else if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			own_laser_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		}
	}
	for (auto& i_opponent_unit : m_opponent_units) {
		if (WorkerUnit* opponent_worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
			opponent_worker_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		} else  if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			opponent_laser_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		}
	}
}

void ClientGame::update(const Timer& timer) {
	if (m_is_started) {
		float delta_time_seconds = timer.get_delta_time_seconds();

		m_map.update(timer);

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
							glm::distance(own_laser_unit->get_position(), i_opponent_unit->get_position()) < min_attack_range) {
							attacked = i_opponent_unit.get();
							min_attack_range = glm::distance(own_laser_unit->get_position(), i_opponent_unit->get_position());
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
			} else if (ShockwaveUnit* own_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_own_unit.get())) {
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
			} else if (WorkerUnit* own_worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
				// Update exploit relationships.
		
				float exploit_range = own_worker_unit->get_attack_range();
				Tile* currently_exploited = own_worker_unit->get_exploited();
				if (currently_exploited) {
					if (glm::distance(own_worker_unit->get_position_vec2(), glm::vec2((currently_exploited->get_x() + 0.5F) * currently_exploited->get_size(), (currently_exploited->get_y() + 0.5F) * currently_exploited->get_size())) > exploit_range ||
						(std::find(m_own_selected_crystal_tiles.begin(), m_own_selected_crystal_tiles.end(), currently_exploited) == m_own_selected_crystal_tiles.end() &&
						std::find(m_own_selected_destructible_rock_tiles.begin(), m_own_selected_destructible_rock_tiles.end(), currently_exploited) == m_own_selected_destructible_rock_tiles.end())) {
						own_worker_unit->stop_exploiting();
						currently_exploited = nullptr;
					}
				}
				if (!currently_exploited) {
					CrystalTile* exploited_crystal = nullptr;
					DestructibleRockTile* exploited_dr = nullptr;
					float min_exploit_range = std::numeric_limits<float>::max();
				
					for (auto& i_tile : m_own_selected_crystal_tiles) {
						if (glm::distance(own_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size())) < exploit_range &&
							glm::distance(own_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size())) < min_exploit_range) {
							exploited_crystal = i_tile;
							min_exploit_range = glm::distance(own_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size()));
						}
					}
					if (exploited_crystal) {
						own_worker_unit->start_exploiting(exploited_crystal);
					} else {
						for (auto& i_tile : m_own_selected_destructible_rock_tiles) {
							if (glm::distance(own_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size())) < exploit_range &&
								glm::distance(own_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size())) < min_exploit_range) {
								exploited_dr = i_tile;
								min_exploit_range = glm::distance(own_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size()));
							}
						}
						if (exploited_dr) {
							own_worker_unit->start_exploiting(exploited_dr);
						}
					}
				}

				// Update health.

				Tile* exploited = own_worker_unit->get_exploited();
				if (exploited) {
					if (CrystalTile* tile = dynamic_cast<CrystalTile*>(exploited)) {
						tile->change_health(delta_time_seconds * -own_worker_unit->get_attack_dps());
						own_worker_unit->change_crystal_count(delta_time_seconds * own_worker_unit->get_attack_dps());
					} else if (DestructibleRockTile* tile = dynamic_cast<DestructibleRockTile*>(exploited)) {
						tile->change_health(delta_time_seconds * -own_worker_unit->get_attack_dps());
					}
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
							glm::distance(opponent_laser_unit->get_position(), i_own_unit->get_position()) < min_attack_range) {
							attacked = i_own_unit.get();
							min_attack_range = glm::distance(opponent_laser_unit->get_position(), i_own_unit->get_position());
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
			} else if (WorkerUnit* opponent_worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
				// Update exploit relationships.
		
				float exploit_range = opponent_worker_unit->get_attack_range();
				Tile* currently_exploited = opponent_worker_unit->get_exploited();
				if (currently_exploited) {
					if (glm::distance(opponent_worker_unit->get_position_vec2(), glm::vec2((currently_exploited->get_x() + 0.5F) * currently_exploited->get_size(), (currently_exploited->get_y() + 0.5F) * currently_exploited->get_size())) > exploit_range ||
						(std::find(m_opponent_selected_crystal_tiles.begin(), m_opponent_selected_crystal_tiles.end(), currently_exploited) == m_opponent_selected_crystal_tiles.end() &&
						std::find(m_opponent_selected_destructible_rock_tiles.begin(), m_opponent_selected_destructible_rock_tiles.end(), currently_exploited) == m_opponent_selected_destructible_rock_tiles.end())) {
						opponent_worker_unit->stop_exploiting();
						currently_exploited = nullptr;
					}
				}
				if (!currently_exploited) {
					CrystalTile* exploited_crystal = nullptr;
					DestructibleRockTile* exploited_dr = nullptr;
					float min_exploit_range = std::numeric_limits<float>::max();
				
					for (auto& i_tile : m_opponent_selected_crystal_tiles) {
						if (glm::distance(opponent_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size())) < exploit_range &&
							glm::distance(opponent_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size())) < min_exploit_range) {
							exploited_crystal = i_tile;
							min_exploit_range = glm::distance(opponent_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size()));
						}
					}
					if (exploited_crystal) {
						opponent_worker_unit->start_exploiting(exploited_crystal);
					} else {
						for (auto& i_tile : m_opponent_selected_destructible_rock_tiles) {
							if (glm::distance(opponent_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size())) < exploit_range &&
								glm::distance(opponent_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size())) < min_exploit_range) {
								exploited_dr = i_tile;
								min_exploit_range = glm::distance(opponent_worker_unit->get_position(), glm::vec3((i_tile->get_x() + 0.5F) * i_tile->get_size(), 0.5F * i_tile->get_size(), (i_tile->get_y() + 0.5F) * i_tile->get_size()));
							}
						}
						if (exploited_dr) {
							opponent_worker_unit->start_exploiting(exploited_dr);
						}
					}
				}

				// Update health.
		
				Tile* exploited = opponent_worker_unit->get_exploited();
				if (exploited) {
					if (CrystalTile* tile = dynamic_cast<CrystalTile*>(exploited)) {
						tile->change_health(delta_time_seconds * -opponent_worker_unit->get_attack_dps());
						opponent_worker_unit->change_crystal_count(delta_time_seconds * opponent_worker_unit->get_attack_dps());
					} else if (DestructibleRockTile* tile = dynamic_cast<DestructibleRockTile*>(exploited)) {
						tile->change_health(delta_time_seconds * -opponent_worker_unit->get_attack_dps());
					}
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
				glm::vec2 position = (*i_own_unit)->get_position_vec2();
				m_explosions.emplace_back(glm::translate(glm::vec3(position.x, 0.2F, position.y)), m_map, 0.75F);
				m_explosions.back().trigger(timer.get_current_time_seconds());
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
				glm::vec2 position = (*i_opponent_unit)->get_position_vec2();
				m_explosions.emplace_back(glm::translate(glm::vec3(position.x, 0.2F, position.y)), m_map, 0.75F);
				m_explosions.back().trigger(timer.get_current_time_seconds());
				i_opponent_unit = m_opponent_units.erase(i_opponent_unit);
			} else {
				++i_opponent_unit;
			}
		}
		for (unsigned int i_y = 0; i_y < m_map.get_tile_count_y(); ++i_y) {
			for (unsigned int i_x = 0; i_x < m_map.get_tile_count_x(); ++i_x) {
				Tile& tile = m_map.get_tile(i_x, i_y);
				Tile* dead_tile = nullptr;
				if (DestructibleRockTile* destructible_rock_tile = dynamic_cast<DestructibleRockTile*>(&tile)) {
					if (destructible_rock_tile->is_dead()) {
						dead_tile = destructible_rock_tile;
						for (auto i_tile = m_own_selected_destructible_rock_tiles.begin(); i_tile != m_own_selected_destructible_rock_tiles.end(); ++i_tile) {
							if (*i_tile == dead_tile) {
								m_own_selected_destructible_rock_tiles.erase(i_tile);
								break;
							}
						}
						for (auto i_tile = m_opponent_selected_destructible_rock_tiles.begin(); i_tile != m_opponent_selected_destructible_rock_tiles.end(); ++i_tile) {
							if (*i_tile == dead_tile) {
								m_opponent_selected_destructible_rock_tiles.erase(i_tile);
								break;
							}
						}
					}
				} else if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(&tile)) {
					if (crystal_tile->is_dead()) {
						dead_tile = crystal_tile;
						for (auto i_tile = m_own_selected_crystal_tiles.begin(); i_tile != m_own_selected_crystal_tiles.end(); ++i_tile) {
							if (*i_tile == dead_tile) {
								m_own_selected_crystal_tiles.erase(i_tile);
								break;
							}
						}
						for (auto i_tile = m_opponent_selected_crystal_tiles.begin(); i_tile != m_opponent_selected_crystal_tiles.end(); ++i_tile) {
							if (*i_tile == dead_tile) {
								m_opponent_selected_crystal_tiles.erase(i_tile);
								break;
							}
						}
					}
				}
				if (dead_tile) {
					for (auto i_own_unit = m_own_units.begin(); i_own_unit != m_own_units.end(); ++i_own_unit) {
						if (WorkerUnit* own_worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit->get())) {
							if (own_worker_unit->get_exploited() == dead_tile) {
								own_worker_unit->stop_exploiting();
							}
						}
					}
					for (auto i_opponent_unit = m_opponent_units.begin(); i_opponent_unit != m_opponent_units.end(); ++i_opponent_unit) {
						if (WorkerUnit* opponent_worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit->get())) {
							if (opponent_worker_unit->get_exploited() == dead_tile) {
								opponent_worker_unit->stop_exploiting();
							}
						}
					}

					m_map.set_tile(std::unique_ptr<Tile>(new FloorTile(m_map, i_x, i_y)));
				}
			}
		}
	
		// Update ressource to base.

		BaseTile* own_base_tile;
		BaseTile* opponent_base_tile;
		for (unsigned int i_y = 0; i_y < m_map.get_tile_count_y(); ++i_y) {
			for (unsigned int i_x = 0; i_x < m_map.get_tile_count_x(); ++i_x) {
				if (BaseTile* base_tile = dynamic_cast<BaseTile*>(&m_map.get_tile(i_x, i_y))) {
					if (&base_tile->get_player() == &m_own_player) {
						own_base_tile = base_tile;
					} else if (&base_tile->get_player() == &m_opponent_player) {
						opponent_base_tile = base_tile;
					}
				}
			}
		}
		for (auto& i_own_unit : m_own_units) {
			if (WorkerUnit* worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
				if (glm::distance(worker_unit->get_position(), own_base_tile->get_position()) <= 3.0F) {
					m_own_crystal_count += worker_unit->get_crystal_count();
					worker_unit->set_crystal_count(0.0F);
				}
			}
		}
		for (auto& i_opponent_unit : m_opponent_units) {
			if (WorkerUnit* worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
				if (glm::distance(worker_unit->get_position(), opponent_base_tile->get_position()) <= 3.0F) {
					m_opponent_crystal_count += worker_unit->get_crystal_count();
					worker_unit->set_crystal_count(0.0F);
				}
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

		static float update_time = 0.0F;
		if (update_time < timer.get_current_time_seconds()) {
			Networkable::on_update();
			update_time = timer.get_current_time_seconds() + 0.05F;
		}






		// CLIENT REVCEIVE

		GameClientHandler* game_client_handler = static_cast<GameClientHandler*>(get_network_handler());
		std::vector<NetworkPacket> network_packets = game_client_handler->receive_incoming_update_network_packets();

		for (auto& i_network_packet : network_packets) {
			std::string text;
			i_network_packet >> text;

			if (text == "ressources") {
				i_network_packet >> m_own_plasma_count;
				i_network_packet >> m_own_crystal_count;
			} else if (text == "new_unit") {
				unsigned int type;
				i_network_packet >> type;
				unsigned int player_id;
				i_network_packet >> player_id;
				float x;
				i_network_packet >> x;
				float y;
				i_network_packet >> y;
				if (type == 0) {
					if (player_id == 0) {
						m_own_units.push_back(WorkerUnit::create(glm::vec2(x, y), m_map, m_own_player));
					} else if (player_id == 1) {
						m_opponent_units.push_back(WorkerUnit::create(glm::vec2(x, y), m_map, m_opponent_player));
					}
				} else if (type == 1) {
					if (player_id == 0) {
						m_own_units.push_back(LaserUnit::create(glm::vec2(x, y), m_map, m_own_player));
					} else if (player_id == 1) {
						m_opponent_units.push_back(LaserUnit::create(glm::vec2(x, y), m_map, m_opponent_player));
					}
				} else if (type == 2) {
					if (player_id == 0) {
						m_own_units.push_back(ShockwaveUnit::create(glm::vec2(x, y), m_map, m_own_player));
					} else if (player_id == 1) {
						m_opponent_units.push_back(ShockwaveUnit::create(glm::vec2(x, y), m_map, m_opponent_player));
					}
				}
			}
		}

		// CLIENT SEND

		NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
		network_packet << std::string("test");
		network_packet << 12.0F;
		m_client.get_participant().add_outgoing_network_packet(network_packet);



	} else {
		Networkable::on_update();
	}
}
