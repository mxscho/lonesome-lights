#include "game/server_game.h"

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

#include "networking/network_handler.h"

float ServerGame::c_worker_unit_plasma_cost = 50.0F;
float ServerGame::c_worker_unit_crystals_cost = 0.0F;
float ServerGame::c_laser_unit_plasma_cost = 100.0F;
float ServerGame::c_laser_unit_crystals_cost = 50.0F;
float ServerGame::c_shockwave_unit_plasma_cost = 150.0F;
float ServerGame::c_shockwave_unit_crystals_cost = 100.0F;

float ServerGame::c_upgrade1_plasma_cost = 100.0F;
float ServerGame::c_upgrade1_crystals_cost = 150.0F;
float ServerGame::c_upgrade2_plasma_cost = 100.0F;
float ServerGame::c_upgrade2_crystals_cost = 150.0F;

float ServerGame::c_plasma_generation = 2.5F;
float ServerGame::c_crystals_generation = 0.0F;

unsigned int ServerGame::c_own_base_x = 27;
unsigned int ServerGame::c_own_base_y = 2;
unsigned int ServerGame::c_opponent_base_x = 2;
unsigned int ServerGame::c_opponent_base_y = 27;

ServerGame::ServerGame(Server& server)
	: Networkable(),
	Updatable(),

	m_own_selected_destructible_rock_tiles(),
	m_own_selected_crystal_tiles(),
	m_opponent_selected_destructible_rock_tiles(),
	m_opponent_selected_crystal_tiles(),

	m_current_unit_id(0),

	m_is_started(false),

	m_map(Map::create_map(1.0F)),
	m_own_player(glm::vec3(0.0F, 0.0F, 0.8F)),
	m_opponent_player(glm::vec3(0.8F, 0.0F, 0.0F)),
	m_own_units(),
	m_opponent_units(),

	m_own_plasma_count(10000.0F),
	m_own_crystal_count(5000.0F),
	m_opponent_plasma_count(10000.0F),
	m_opponent_crystal_count(5000.0F),

	m_server(server) {

	m_map.set_tile(std::unique_ptr<Tile>(new BaseTile(BaseTile::create(m_map, c_own_base_x, c_own_base_y, m_own_player))));
	Tile& base_up_own = m_map.get_tile(c_own_base_x, c_own_base_y + 1);
	Tile& base_down_own = m_map.get_tile(c_own_base_x, c_own_base_y - 1);
	Tile& base_left_own = m_map.get_tile(c_own_base_x - 1, c_own_base_y);
	Tile& base_right_own = m_map.get_tile(c_own_base_x + 1, c_own_base_y);
	base_up_own.set_is_walkable(false);
	base_down_own.set_is_walkable(false);
	base_left_own.set_is_walkable(false);
	base_right_own.set_is_walkable(false);
	m_map.set_tile(std::unique_ptr<Tile>(new BaseTile(BaseTile::create(m_map, c_opponent_base_x, c_opponent_base_y, m_opponent_player))));
	Tile& base_up_opponent = m_map.get_tile(c_opponent_base_x, c_opponent_base_y + 1);
	Tile& base_down_opponent = m_map.get_tile(c_opponent_base_x, c_opponent_base_y - 1);
	Tile& base_left_opponent = m_map.get_tile(c_opponent_base_x - 1, c_opponent_base_y);
	Tile& base_right_opponent = m_map.get_tile(c_opponent_base_x + 1, c_opponent_base_y);
	base_up_opponent.set_is_walkable(false);
	base_down_opponent.set_is_walkable(false);
	base_left_opponent.set_is_walkable(false);
	base_right_opponent.set_is_walkable(false);

	m_own_units.push_back(WorkerUnit::create(glm::vec2(c_own_base_x - 1, c_own_base_y + 1), m_map, m_own_player, 1000000U));
	m_opponent_units.push_back(WorkerUnit::create(glm::vec2(c_opponent_base_x + 1, c_opponent_base_y - 1), m_map, m_opponent_player, 1000001U));
}

bool ServerGame::has_started() const {
	return m_is_started;
}
void ServerGame::start() {
	m_is_started = true;
}

float ServerGame::get_own_plasma_count() const {
	return m_own_plasma_count;
}
float ServerGame::get_own_crystal_count() const {
	return m_own_crystal_count;
}

Map& ServerGame::get_map() {
	return m_map;
}
std::list<std::unique_ptr<Unit>>& ServerGame::get_own_units() {
	return m_own_units;
}
std::list<std::unique_ptr<Unit>>& ServerGame::get_opponent_units() {
	return m_opponent_units;
}

void ServerGame::spawn_own_worker_unit() {
	if (m_own_plasma_count < c_worker_unit_plasma_cost || m_own_crystal_count < c_worker_unit_crystals_cost) {
		return;
	}
	m_own_plasma_count -= c_worker_unit_plasma_cost;
	m_own_crystal_count -= c_worker_unit_crystals_cost;
	m_own_units.push_back(WorkerUnit::create(glm::vec2(c_own_base_x, c_own_base_y), m_map, m_own_player, m_current_unit_id));

	// SERVER SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 0U;
	network_packet << 0U;
	network_packet << static_cast<float>(c_own_base_x - 1);
	network_packet << static_cast<float>(c_own_base_y + 1);
	m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
	network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 0U;
	network_packet << 0U;
	network_packet << static_cast<float>(c_own_base_x - 1);
	network_packet << static_cast<float>(c_own_base_y + 1);
	m_server.get_participants()[1].add_outgoing_network_packet(network_packet);

	m_current_unit_id++;
}
void ServerGame::spawn_own_laser_unit() {
	if (m_own_plasma_count < c_laser_unit_plasma_cost || m_own_crystal_count < c_laser_unit_crystals_cost) {
		return;
	}
	m_own_plasma_count -= c_laser_unit_plasma_cost;
	m_own_crystal_count -= c_laser_unit_crystals_cost;
	m_own_units.push_back(LaserUnit::create(glm::vec2(c_own_base_x, c_own_base_y), m_map, m_own_player, m_current_unit_id));

	// SERVER SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 1U;
	network_packet << 0U;
	network_packet << static_cast<float>(c_own_base_x - 1);
	network_packet << static_cast<float>(c_own_base_y + 1);
	m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
	network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 1U;
	network_packet << 0U;
	network_packet << static_cast<float>(c_own_base_x - 1);
	network_packet << static_cast<float>(c_own_base_y + 1);
	m_server.get_participants()[1].add_outgoing_network_packet(network_packet);

	m_current_unit_id++;
}
void ServerGame::spawn_own_shockwave_unit() {
	if (m_own_plasma_count < c_shockwave_unit_plasma_cost || m_own_crystal_count < c_shockwave_unit_crystals_cost) {
		return;
	}
	m_own_plasma_count -= c_shockwave_unit_plasma_cost;
	m_own_crystal_count -= c_shockwave_unit_crystals_cost;
	m_own_units.push_back(ShockwaveUnit::create(glm::vec2(c_own_base_x, c_own_base_y), m_map, m_own_player, m_current_unit_id));

	// SERVER SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 2U;
	network_packet << 0U;
	network_packet << static_cast<float>(c_own_base_x - 1);
	network_packet << static_cast<float>(c_own_base_y + 1);
	m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
	network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 2U;
	network_packet << 0U;
	network_packet << static_cast<float>(c_own_base_x - 1);
	network_packet << static_cast<float>(c_own_base_y + 1);
	m_server.get_participants()[1].add_outgoing_network_packet(network_packet);

	m_current_unit_id++;
}

void ServerGame::spawn_opponent_worker_unit() {
	if (m_opponent_plasma_count < c_worker_unit_plasma_cost || m_opponent_crystal_count < c_worker_unit_crystals_cost) {
		return;
	}
	m_opponent_plasma_count -= c_worker_unit_plasma_cost;
	m_opponent_crystal_count -= c_worker_unit_crystals_cost;
	m_opponent_units.push_back(WorkerUnit::create(glm::vec2(c_opponent_base_x, c_opponent_base_y), m_map, m_opponent_player, m_current_unit_id));

	// SERVER SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 0U;
	network_packet << 1U;
	network_packet << static_cast<float>(c_opponent_base_x + 1);
	network_packet << static_cast<float>(c_opponent_base_y - 1);
	m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
	network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 0U;
	network_packet << 1U;
	network_packet << static_cast<float>(c_opponent_base_x + 1);
	network_packet << static_cast<float>(c_opponent_base_y - 1);
	m_server.get_participants()[1].add_outgoing_network_packet(network_packet);

	m_current_unit_id++;
}
void ServerGame::spawn_opponent_laser_unit() {
	if (m_opponent_plasma_count < c_laser_unit_plasma_cost || m_opponent_crystal_count < c_laser_unit_crystals_cost) {
		return;
	}
	m_opponent_plasma_count -= c_laser_unit_plasma_cost;
	m_opponent_crystal_count -= c_laser_unit_crystals_cost;
	m_opponent_units.push_back(LaserUnit::create(glm::vec2(c_opponent_base_x, c_opponent_base_y), m_map, m_opponent_player, m_current_unit_id));

	// SERVER SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 1U;
	network_packet << 1U;
	network_packet << static_cast<float>(c_opponent_base_x + 1);
	network_packet << static_cast<float>(c_opponent_base_y - 1);
	m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
	network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 1U;
	network_packet << 1U;
	network_packet << static_cast<float>(c_opponent_base_x + 1);
	network_packet << static_cast<float>(c_opponent_base_y - 1);
	m_server.get_participants()[1].add_outgoing_network_packet(network_packet);

	m_current_unit_id++;
}
void ServerGame::spawn_opponent_shockwave_unit() {
	if (m_opponent_plasma_count < c_shockwave_unit_plasma_cost || m_opponent_crystal_count < c_shockwave_unit_crystals_cost) {
		return;
	}
	m_opponent_plasma_count -= c_shockwave_unit_plasma_cost;
	m_opponent_crystal_count -= c_shockwave_unit_crystals_cost;
	m_opponent_units.push_back(ShockwaveUnit::create(glm::vec2(c_opponent_base_x, c_opponent_base_y), m_map, m_opponent_player, m_current_unit_id));

	// SERVER SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 2U;
	network_packet << 1U;
	network_packet << static_cast<float>(c_opponent_base_x + 1);
	network_packet << static_cast<float>(c_opponent_base_y - 1);
	m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
	network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << m_current_unit_id;
	network_packet << 2U;
	network_packet << 1U;
	network_packet << static_cast<float>(c_opponent_base_x + 1);
	network_packet << static_cast<float>(c_opponent_base_y - 1);
	m_server.get_participants()[1].add_outgoing_network_packet(network_packet);

	m_current_unit_id++;
}

void ServerGame::update(const Timer& timer) {
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
					Tile& tile = m_map.get_tile(c_opponent_base_x, c_opponent_base_y);
					BaseTile* base_tile = static_cast<BaseTile*>(&tile);
					if (glm::distance(own_laser_unit->get_position_vec2(), base_tile->get_position_vec2()) < attack_range) {
						own_laser_unit->start_shooting(base_tile);
					} else if (attacked) {
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
				Tile& tile = m_map.get_tile(c_opponent_base_x, c_opponent_base_y);
				BaseTile* base_tile = static_cast<BaseTile*>(&tile);
				if (glm::distance(own_shockwave_unit->get_position_vec2(), base_tile->get_position_vec2()) < attack_range) {
					own_shockwave_unit->add_attack(base_tile);
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
					Tile& tile = m_map.get_tile(c_own_base_x, c_own_base_y);
					BaseTile* base_tile = static_cast<BaseTile*>(&tile);
					if (glm::distance(opponent_laser_unit->get_position_vec2(), base_tile->get_position_vec2()) < attack_range) {
						opponent_laser_unit->start_shooting(base_tile);
					} else if (attacked) {
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
				Tile& tile = m_map.get_tile(c_own_base_x, c_own_base_y);
				BaseTile* base_tile = static_cast<BaseTile*>(&tile);
				if (glm::distance(opponent_shockwave_unit->get_position_vec2(), base_tile->get_position_vec2()) < attack_range) {
					opponent_shockwave_unit->add_attack(base_tile);
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

		// Update ressources.

		m_own_plasma_count += delta_time_seconds * c_plasma_generation;
		m_opponent_plasma_count += delta_time_seconds * c_plasma_generation;
		m_own_crystal_count += delta_time_seconds * c_crystals_generation;
		m_opponent_crystal_count += delta_time_seconds * c_crystals_generation;
	
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

		static float update_time = 0.0F;
		if (update_time < timer.get_current_time_seconds()) {
			Networkable::on_update();
			update_time = timer.get_current_time_seconds() + 0.05F;
		}		

		// SERVER RECEIVE

		std::vector<NetworkPacket> network_packets;

		std::vector<NetworkPacket>& incoming_network_packets_own = m_server.get_participants()[0].get_incoming_network_packets();
		for (auto i_network_packet = incoming_network_packets_own.begin(); i_network_packet != incoming_network_packets_own.end(); ) {
			if (i_network_packet->get_network_id() == get_network_handler()->m_network_id && i_network_packet->get_type() == NetworkPacket::Type::Update) {
				network_packets.push_back(*i_network_packet);
				i_network_packet = incoming_network_packets_own.erase(i_network_packet);
			} else {
				++i_network_packet;
			}
		}

		for (auto& i_network_packet : network_packets) {
			std::string text;
			i_network_packet >> text;

			if (text == "new_unit") {
				unsigned int type;
				i_network_packet >> type;
				if (type == 0U) {
					spawn_own_worker_unit();
				} else if (type == 1U) {
					spawn_own_laser_unit();
				} else if (type == 2U) {
					spawn_own_shockwave_unit();
				}
			} else if (text == "move_unit") {
				unsigned int id;
				i_network_packet >> id;
				for (auto& i_own_unit : m_own_units) {
					if (i_own_unit->m_id == id) {
						unsigned int size;
						i_network_packet >> size;
						std::list<glm::vec3> target_path;
						for (unsigned int i = 0; i < size; ++i) {
							float x;
							i_network_packet >> x;
							float y;
							i_network_packet >> y;
							float z;
							i_network_packet >> z;
							target_path.push_back(glm::vec3(x, y, z));
						}
						i_own_unit->InertialMovable::set_target_path(timer, target_path);

						// SERVER SEND

						NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
						network_packet << std::string("move_unit");
						network_packet << i_own_unit->m_id;
						network_packet << static_cast<unsigned int>(target_path.size());
						for (auto& i_target_position : target_path) {
							network_packet << i_target_position.x;
							network_packet << i_target_position.y;
							network_packet << i_target_position.z;
						}

						m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
					}
				}
			} else if (text == "upgrade_damage") {
				if (m_own_plasma_count >= c_upgrade1_plasma_cost && m_own_crystal_count >= c_upgrade1_crystals_cost) {
					for (auto& i_own_unit : m_own_units) {
						i_own_unit->m_damage_upgrade = true;
					}
					m_own_plasma_count -= c_upgrade1_plasma_cost;
					m_own_crystal_count -= c_upgrade1_crystals_cost;
				}
				NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
				network_packet << std::string("upgrade_damage");
				m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
			} else if (text == "upgrade_range") {
				if (m_own_plasma_count >= c_upgrade2_plasma_cost && m_own_crystal_count >= c_upgrade2_crystals_cost) {
					for (auto& i_own_unit : m_own_units) {
						i_own_unit->m_range_upgrade = true;
					}
					m_own_plasma_count -= c_upgrade2_plasma_cost;
					m_own_crystal_count -= c_upgrade2_crystals_cost;
				}
				NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
				network_packet << std::string("upgrade_range");
				m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
			} else if (text == "select_tiles") {
				m_own_selected_destructible_rock_tiles.clear();
				unsigned int size;
				i_network_packet >> size;
				for (unsigned int i = 0; i < size; ++ i) {
					unsigned int x;
					i_network_packet >> x;
					unsigned int y;
					i_network_packet >> y;
					if (DestructibleRockTile* tile = dynamic_cast<DestructibleRockTile*>(&m_map.get_tile(x, y))) {
						m_own_selected_destructible_rock_tiles.push_back(tile);
					}
				}
				m_own_selected_crystal_tiles.clear();
				i_network_packet >> size;
				for (unsigned int i = 0; i < size; ++ i) {
					unsigned int x;
					i_network_packet >> x;
					unsigned int y;
					i_network_packet >> y;
					if (CrystalTile* tile = dynamic_cast<CrystalTile*>(&m_map.get_tile(x, y))) {
						m_own_selected_crystal_tiles.push_back(tile);
					}
				}
			}
		}

		network_packets.clear();
		std::vector<NetworkPacket>& incoming_network_packets_opponent = m_server.get_participants()[1].get_incoming_network_packets();
		for (auto i_network_packet = incoming_network_packets_opponent.begin(); i_network_packet != incoming_network_packets_opponent.end(); ) {
			if (i_network_packet->get_network_id() == get_network_handler()->m_network_id && i_network_packet->get_type() == NetworkPacket::Type::Update) {
				network_packets.push_back(*i_network_packet);
				i_network_packet = incoming_network_packets_opponent.erase(i_network_packet);
			} else {
				++i_network_packet;
			}
		}

		for (auto& i_network_packet : network_packets) {
			std::string text;
			i_network_packet >> text;

			if (text == "new_unit") {
				unsigned int type;
				i_network_packet >> type;
				if (type == 0U) {
					spawn_opponent_worker_unit();
				} else if (type == 1U) {
					spawn_opponent_laser_unit();
				} else if (type == 2U) {
					spawn_opponent_shockwave_unit();
				}
			} else if (text == "move_unit") {
				unsigned int id;
				i_network_packet >> id;
				for (auto& i_opponent_unit : m_opponent_units) {
					if (i_opponent_unit->m_id == id) {
						unsigned int size;
						i_network_packet >> size;
						std::list<glm::vec3> target_path;
						for (unsigned int i = 0; i < size; ++i) {
							float x;
							i_network_packet >> x;
							float y;
							i_network_packet >> y;
							float z;
							i_network_packet >> z;
							target_path.push_back(glm::vec3(x, y, z));
						}
						i_opponent_unit->InertialMovable::set_target_path(timer, target_path);

						// SERVER SEND

						NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
						network_packet << std::string("move_unit");
						network_packet << i_opponent_unit->m_id;
						network_packet << static_cast<unsigned int>(target_path.size());
						for (auto& i_target_position : target_path) {
							network_packet << i_target_position.x;
							network_packet << i_target_position.y;
							network_packet << i_target_position.z;
						}

						m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
					}
				}
			} if (text == "upgrade_damage") {
				if (m_opponent_plasma_count >= c_upgrade1_plasma_cost && m_opponent_crystal_count >= c_upgrade1_crystals_cost) {
					for (auto& i_opponent_unit : m_opponent_units) {
						i_opponent_unit->m_damage_upgrade = true;
					}
					m_opponent_plasma_count -= c_upgrade1_plasma_cost;
					m_opponent_crystal_count -= c_upgrade1_crystals_cost;
					NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
					network_packet << std::string("upgrade_damage");
					m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
				}
			} else if (text == "upgrade_range") {
				if (m_opponent_plasma_count >= c_upgrade2_plasma_cost && m_opponent_crystal_count >= c_upgrade2_crystals_cost) {
					for (auto& i_opponent_unit : m_opponent_units) {
						i_opponent_unit->m_range_upgrade = true;
					}
					m_opponent_plasma_count -= c_upgrade2_plasma_cost;
					m_opponent_crystal_count -= c_upgrade2_crystals_cost;
					NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
					network_packet << std::string("upgrade_range");
					m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
				}
			} else if (text == "select_tiles") {
				m_opponent_selected_destructible_rock_tiles.clear();
				unsigned int size;
				i_network_packet >> size;
				for (unsigned int i = 0; i < size; ++ i) {
					unsigned int x;
					i_network_packet >> x;
					unsigned int y;
					i_network_packet >> y;
					if (DestructibleRockTile* tile = dynamic_cast<DestructibleRockTile*>(&m_map.get_tile(x, y))) {
						m_opponent_selected_destructible_rock_tiles.push_back(tile);
					}
				}
				m_opponent_selected_crystal_tiles.clear();
				i_network_packet >> size;
				for (unsigned int i = 0; i < size; ++ i) {
					unsigned int x;
					i_network_packet >> x;
					unsigned int y;
					i_network_packet >> y;
					if (CrystalTile* tile = dynamic_cast<CrystalTile*>(&m_map.get_tile(x, y))) {
						m_opponent_selected_crystal_tiles.push_back(tile);
					}
				}
			}
		}

		static float time = 0.0F;
		if (time >= 0.1F) {
			time = 0.0F;

			// Update deaths (attacks).

			for (auto i_own_unit = m_own_units.begin(); i_own_unit != m_own_units.end(); ++i_own_unit) {
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
				}
			}
			for (auto i_opponent_unit = m_opponent_units.begin(); i_opponent_unit != m_opponent_units.end(); ++i_opponent_unit) {
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
				}
			}
			for (unsigned int i_y = 0; i_y < m_map.get_tile_count_y(); ++i_y) {
				for (unsigned int i_x = 0; i_x < m_map.get_tile_count_x(); ++i_x) {
					Tile& tile = m_map.get_tile(i_x, i_y);
					Tile* dead_tile = nullptr;
					if (DestructibleRockTile* destructible_rock_tile = dynamic_cast<DestructibleRockTile*>(&tile)) {
						if (destructible_rock_tile->is_dead()) {
							dead_tile = destructible_rock_tile;
						}
					} else if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(&tile)) {
						if (crystal_tile->is_dead()) {
							dead_tile = crystal_tile;
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
					} else if (BaseTile* base_tile = dynamic_cast<BaseTile*>(&tile)) {
						if (base_tile->is_dead()) {
							for (auto i_own_unit = m_own_units.begin(); i_own_unit != m_own_units.end(); ++i_own_unit) {
								if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit->get())) {
									if (own_laser_unit->get_shooting_target() == base_tile) {
										own_laser_unit->stop_shooting();
									}
								} else if (ShockwaveUnit* own_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_own_unit->get())) {
									own_shockwave_unit->remove_attack(base_tile);
								}
							}
							for (auto i_opponent_unit = m_opponent_units.begin(); i_opponent_unit != m_opponent_units.end(); ++i_opponent_unit) {
								if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit->get())) {
									if (opponent_laser_unit->get_shooting_target() == base_tile) {
										opponent_laser_unit->stop_shooting();
									}
								} else if (ShockwaveUnit* opponent_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_opponent_unit->get())) {
									opponent_shockwave_unit->remove_attack(base_tile);
								}
							}
						}
					}
				}
			}

			// SERVER SEND

			NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
			network_packet << std::string("ressources");
			network_packet << m_own_plasma_count;
			network_packet << m_own_crystal_count;
			m_server.get_participants()[0].add_outgoing_network_packet(network_packet);

			network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
			network_packet << std::string("ressources");
			network_packet << m_opponent_plasma_count;
			network_packet << m_opponent_crystal_count;
			m_server.get_participants()[1].add_outgoing_network_packet(network_packet);

			for (auto& i_own_unit : m_own_units) {
				network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
				network_packet << std::string("unit_health");
				network_packet << i_own_unit->m_id;
				network_packet << i_own_unit->get_health();
				m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
				m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
			}
			for (auto& i_opponent_unit : m_opponent_units) {
				network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
				network_packet << std::string("unit_health");
				network_packet << i_opponent_unit->m_id;
				network_packet << i_opponent_unit->get_health();
				m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
				m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
			}

			for (auto& i_own_unit : m_own_units) {
				if (WorkerUnit* worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
					network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
					network_packet << std::string("worker_crystals");
					network_packet << i_own_unit->m_id;
					network_packet << worker_unit->get_crystal_count();
					m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
					m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
				}
			}
			for (auto& i_opponent_unit : m_opponent_units) {
				if (WorkerUnit* worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
					network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
					network_packet << std::string("worker_crystals");
					network_packet << i_opponent_unit->m_id;
					network_packet << worker_unit->get_crystal_count();
					m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
					m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
				}
			}

			for (unsigned int i_y = 0; i_y < m_map.get_tile_count_y(); ++i_y) {
				for (unsigned int i_x = 0; i_x < m_map.get_tile_count_x(); ++i_x) {
					Tile& tile = m_map.get_tile(i_x, i_y);
					if (DestructibleRockTile* destructible_rock_tile = dynamic_cast<DestructibleRockTile*>(&tile)) {
						network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
						network_packet << std::string("tile_health");
						network_packet << i_x;
						network_packet << i_y;
						network_packet << destructible_rock_tile->get_health();
						m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
						m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
					} else if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(&tile)) {
						network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
						network_packet << std::string("tile_health");
						network_packet << i_x;
						network_packet << i_y;
						network_packet << crystal_tile->get_health();
						m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
						m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
					} else if (BaseTile* base_tile = dynamic_cast<BaseTile*>(&tile)) {
						network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
						network_packet << std::string("tile_health");
						network_packet << i_x;
						network_packet << i_y;
						network_packet << base_tile->get_health();
						m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
						m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
					}
				}
			}

			// Update deaths (removal).

			for (auto i_own_unit = m_own_units.begin(); i_own_unit != m_own_units.end(); ) {
				if ((*i_own_unit)->is_dead()) {
					i_own_unit = m_own_units.erase(i_own_unit);
				} else {
					++i_own_unit;
				}
			}
			for (auto i_opponent_unit = m_opponent_units.begin(); i_opponent_unit != m_opponent_units.end(); ) {
				if ((*i_opponent_unit)->is_dead()) {
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
						m_map.set_tile(std::unique_ptr<Tile>(new FloorTile(m_map, i_x, i_y)));
						m_map.update_neighbors_of_tile(i_x, i_y);
					}
				}
			}

			// SERVER SEND

			for (auto& i_own_unit : m_own_units) {
				NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);				
				network_packet << std::string("unit_attack");
				network_packet << i_own_unit->m_id;
				if (LaserUnit* laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
					Attackable* attackable = laser_unit->get_shooting_target();
					if (attackable) {
						network_packet << true;
						if (Unit* attacked = dynamic_cast<Unit*>(attackable)) {
							network_packet << 0U;
							network_packet << attacked->m_id;
						} else if (BaseTile* attacked = dynamic_cast<BaseTile*>(attackable)) {
							network_packet << 1U;
							network_packet << attacked->get_x();
							network_packet << attacked->get_y();
						}
					} else {
						network_packet << false;
					}		
				} else if (ShockwaveUnit* shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_own_unit.get())) {
					const std::set<Attackable*>& attacked = shockwave_unit->get_attacked();
					network_packet << static_cast<unsigned int>(attacked.size());
					for (auto& i_attacked : attacked) {
						if (Unit* attacked_unit = dynamic_cast<Unit*>(i_attacked)) {
							network_packet << 0U;
							network_packet << attacked_unit->m_id;
						} else if (BaseTile* attacked = dynamic_cast<BaseTile*>(i_attacked)) {
							network_packet << 1U;
							network_packet << attacked->get_x();
							network_packet << attacked->get_y();
						}
					}		
				} else if (WorkerUnit* worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
					Tile* exploited = worker_unit->get_exploited();
					if (exploited) {
						network_packet << true;
						network_packet << exploited->get_x();
						network_packet << exploited->get_y();
					} else {
						network_packet << false;
					}
				}
				m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
				m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
			}
			for (auto& i_opponent_unit : m_opponent_units) {
				NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);				
				network_packet << std::string("unit_attack");
				network_packet << i_opponent_unit->m_id;
				if (LaserUnit* laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
					Attackable* attackable = laser_unit->get_shooting_target();
					if (attackable) {
						network_packet << true;
						if (Unit* attacked = dynamic_cast<Unit*>(attackable)) {
							network_packet << 0U;
							network_packet << attacked->m_id;
						} else if (BaseTile* attacked = dynamic_cast<BaseTile*>(attackable)) {
							network_packet << 1U;
							network_packet << attacked->get_x();
							network_packet << attacked->get_y();
						}
					} else {
						network_packet << false;
					}		
				} else if (ShockwaveUnit* shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_opponent_unit.get())) {
					const std::set<Attackable*>& attacked = shockwave_unit->get_attacked();
					network_packet << static_cast<unsigned int>(attacked.size());
					for (auto& i_attacked : attacked) {
						if (Unit* attacked_unit = dynamic_cast<Unit*>(i_attacked)) {
							network_packet << 0U;
							network_packet << attacked_unit->m_id;
						} else if (BaseTile* attacked = dynamic_cast<BaseTile*>(i_attacked)) {
							network_packet << 1U;
							network_packet << attacked->get_x();
							network_packet << attacked->get_y();
						}
					}		
				} else if (WorkerUnit* worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
					Tile* exploited = worker_unit->get_exploited();
					if (exploited) {
						network_packet << true;
						network_packet << exploited->get_x();
						network_packet << exploited->get_y();
					} else {
						network_packet << false;
					}
				}
				m_server.get_participants()[0].add_outgoing_network_packet(network_packet);
				m_server.get_participants()[1].add_outgoing_network_packet(network_packet);
			}
		} else {
			time += delta_time_seconds;
		}
	} else {
		Networkable::on_update();
	}
}
