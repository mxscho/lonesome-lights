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

	m_is_won(false),

	m_is_started(false),

	m_map(Map::create_map(1.0F)),
	m_own_player(glm::vec3(0.0F, 0.0F, 0.8F)),
	m_opponent_player(glm::vec3(0.8F, 0.0F, 0.0F)),
	m_own_units(),
	m_opponent_units(),

	m_explosions(),
	m_smokes(),
	m_client(client) {

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
	network_packet << 0U;
	m_client.get_participant().add_outgoing_network_packet(network_packet);
}
void ClientGame::spawn_own_laser_unit() {
	// CLIENT SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << 1U;
	m_client.get_participant().add_outgoing_network_packet(network_packet);
}
void ClientGame::spawn_own_shockwave_unit() {
	// CLIENT SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("new_unit");
	network_packet << 2U;
	m_client.get_participant().add_outgoing_network_packet(network_packet);
}

void ClientGame::upgrade_damage() {
	// CLIENT SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("upgrade_damage");
	m_client.get_participant().add_outgoing_network_packet(network_packet);
}
void ClientGame::upgrade_range() {
	// CLIENT SEND

	NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
	network_packet << std::string("upgrade_range");
	m_client.get_participant().add_outgoing_network_packet(network_packet);
}

void ClientGame::draw(const Camera& camera) const {
	m_map.draw(camera);
	
	// Draw explosions.
	for (auto& i_explosion : m_explosions) {
		i_explosion.draw(camera);
	}
	for (auto& i_smoke : m_smokes) {
		i_smoke.draw(camera);
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
	m_map.draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
	for (auto& i_own_unit : m_own_units) {
		if (WorkerUnit* own_worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
			own_worker_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		} else if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			own_laser_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		} else if (ShockwaveUnit* own_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_own_unit.get())) {
			own_shockwave_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		}
	}
	for (auto& i_opponent_unit : m_opponent_units) {
		if (WorkerUnit* opponent_worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
			opponent_worker_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		} else  if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			opponent_laser_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		} else if (ShockwaveUnit* opponent_shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_opponent_unit.get())) {
			opponent_shockwave_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		}
	}
}

void ClientGame::update(const Timer& timer) {
	if (m_is_started) {
		//static bool first_time = true;
		//if (first_time) {
		//	first_time = false;
		//	spawn_own_worker_unit();
		//}

		m_map.update(timer);

		// CLIENT RECEIVE

		std::vector<NetworkPacket> network_packets;

		std::vector<NetworkPacket>& incoming_network_packets = m_client.get_participant().get_incoming_network_packets();
		for (auto i_network_packet = incoming_network_packets.begin(); i_network_packet != incoming_network_packets.end(); ) {
			if (i_network_packet->get_network_id() == get_network_handler()->m_network_id && i_network_packet->get_type() == NetworkPacket::Type::Update) {
				network_packets.push_back(*i_network_packet);
				i_network_packet = incoming_network_packets.erase(i_network_packet);
			} else {
				++i_network_packet;
			}
		}

		for (auto& i_network_packet : network_packets) {
			std::string text;
			i_network_packet >> text;

			if (text == "ressources") {
				i_network_packet >> m_own_plasma_count;
				i_network_packet >> m_own_crystal_count;
			} else if (text == "new_unit") {
				unsigned int id;
				i_network_packet >> id;
				unsigned int type;
				i_network_packet >> type;
				unsigned int player_id;
				i_network_packet >> player_id;
				float x;
				i_network_packet >> x;
				float y;
				i_network_packet >> y;
				if (type == 0U) {
					if (player_id == 0U) {
						m_own_units.push_back(WorkerUnit::create(glm::vec2(x, y), m_map, m_own_player, id));
					} else if (player_id == 1U) {
						m_opponent_units.push_back(WorkerUnit::create(glm::vec2(x, y), m_map, m_opponent_player, id));
					}
				} else if (type == 1U) {
					if (player_id == 0U) {
						m_own_units.push_back(LaserUnit::create(glm::vec2(x, y), m_map, m_own_player, id));
					} else if (player_id == 1U) {
						m_opponent_units.push_back(LaserUnit::create(glm::vec2(x, y), m_map, m_opponent_player, id));
					}
				} else if (type == 2U) {
					if (player_id == 0U) {
						m_own_units.push_back(ShockwaveUnit::create(glm::vec2(x, y), m_map, m_own_player, id));
					} else if (player_id == 1U) {
						m_opponent_units.push_back(ShockwaveUnit::create(glm::vec2(x, y), m_map, m_opponent_player, id));
					}
				}
			} else if (text == "upgrade_damage") {
				HUD::m_used_upgrade1 = true;
			} else if (text == "upgrade_range") {
				HUD::m_used_upgrade2 = true;
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
					}
				}
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
					}
				}
			} else if (text == "worker_crystals") {
				unsigned int id;
				i_network_packet >> id;
				float crystal_count;
				i_network_packet >> crystal_count;

				for (auto& i_own_unit : m_own_units) {
					if (i_own_unit->m_id == id) {
						static_cast<WorkerUnit*>(i_own_unit.get())->set_crystal_count(crystal_count);
					}
				}
				for (auto& i_opponent_unit : m_opponent_units) {
					if (i_opponent_unit->m_id == id) {
						static_cast<WorkerUnit*>(i_opponent_unit.get())->set_crystal_count(crystal_count);
					}
				}
			} else if (text == "tile_health") {
				unsigned int x;
				i_network_packet >> x;
				unsigned int y;
				i_network_packet >> y;
				float health;
				i_network_packet >> health;		
					
				Tile& tile = m_map.get_tile(x, y);
				if (DestructibleRockTile* destructible_rock_tile = dynamic_cast<DestructibleRockTile*>(&tile)) {
					destructible_rock_tile->set_health(health);
				} else if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(&tile)) {
					crystal_tile->set_health(health);
				} else if (BaseTile* base_tile = dynamic_cast<BaseTile*>(&tile)) {
					base_tile->set_health(health);
				}
			} else if (text == "unit_attack") {
				unsigned int id;
				i_network_packet >> id;

				for (auto& i_own_unit : m_own_units) {
					if (i_own_unit->m_id == id) {
						if (LaserUnit* laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
							bool has_attackable;
							i_network_packet >> has_attackable;
							if (has_attackable) {
								unsigned int attacked_type;
								i_network_packet >> attacked_type;
								if (attacked_type == 0U) {
									unsigned int attacked_id;
									i_network_packet >> attacked_id;
									for (auto& i_opponent_unit : m_opponent_units) {
										if (attacked_id == i_opponent_unit->m_id) {
											laser_unit->start_shooting(static_cast<Attackable*>(i_opponent_unit.get()));
										}
									}
								} else if (attacked_type == 1U) {
									unsigned int attacked_x;
									i_network_packet >> attacked_x;
									unsigned int attacked_y;
									i_network_packet >> attacked_y;
									Tile& tile = m_map.get_tile(attacked_x, attacked_y);
									if (BaseTile* base_tile = dynamic_cast<BaseTile*>(&tile)) {
										laser_unit->start_shooting(static_cast<Attackable*>(base_tile));
									}
								}
							} else {
								laser_unit->stop_shooting();
							}	
						} else if (ShockwaveUnit* shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_own_unit.get())) {
							unsigned int size;
							i_network_packet >> size;
							shockwave_unit->clear_attacks();
							for (unsigned int i = 0; i < size; ++i) {
								unsigned int attacked_type;
								i_network_packet >> attacked_type;
								if (attacked_type == 0U) {
									unsigned int attacked_id;
									i_network_packet >> attacked_id;
									for (auto& i_opponent_unit : m_opponent_units) {
										if (attacked_id == i_opponent_unit->m_id) {
											shockwave_unit->add_attack(static_cast<Attackable*>(i_opponent_unit.get()));
										}
									}
								} else if (attacked_type == 1U) {
									unsigned int attacked_x;
									i_network_packet >> attacked_x;
									unsigned int attacked_y;
									i_network_packet >> attacked_y;
									Tile& tile = m_map.get_tile(attacked_x, attacked_y);
									if (BaseTile* base_tile = dynamic_cast<BaseTile*>(&tile)) {
										shockwave_unit->add_attack(static_cast<Attackable*>(base_tile));
									}
								}
							}	
						} else if (WorkerUnit* worker_unit = dynamic_cast<WorkerUnit*>(i_own_unit.get())) {
							bool has_exploited;
							i_network_packet >> has_exploited;
							if (has_exploited) {
								unsigned int x;
								i_network_packet >> x;
								unsigned int y;
								i_network_packet >> y;
								Tile& tile = m_map.get_tile(x, y);
								if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(&tile)) {
									worker_unit->start_exploiting(crystal_tile);
								} else if (DestructibleRockTile* destructible_rock_tile = dynamic_cast<DestructibleRockTile*>(&tile)) {
									worker_unit->start_exploiting(destructible_rock_tile);
								}
							} else {
								worker_unit->stop_exploiting();
							}	
						}
					}
				}
				for (auto& i_opponent_unit : m_opponent_units) {
					if (i_opponent_unit->m_id == id) {
						if (LaserUnit* laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
							bool has_attackable;
							i_network_packet >> has_attackable;
							if (has_attackable) {
								unsigned int attacked_type;
								i_network_packet >> attacked_type;
								if (attacked_type == 0U) {
									unsigned int attacked_id;
									i_network_packet >> attacked_id;
									for (auto& i_own_unit : m_own_units) {
										if (attacked_id == i_own_unit->m_id) {
											laser_unit->start_shooting(static_cast<Attackable*>(i_own_unit.get()));
										}
									}
								} else if (attacked_type == 1U) {
									unsigned int attacked_x;
									i_network_packet >> attacked_x;
									unsigned int attacked_y;
									i_network_packet >> attacked_y;
									Tile& tile = m_map.get_tile(attacked_x, attacked_y);
									if (BaseTile* base_tile = dynamic_cast<BaseTile*>(&tile)) {
										laser_unit->start_shooting(static_cast<Attackable*>(base_tile));
									}
								}
							} else {
								laser_unit->stop_shooting();
							}	
						} else if (ShockwaveUnit* shockwave_unit = dynamic_cast<ShockwaveUnit*>(i_opponent_unit.get())) {
							unsigned int size;
							i_network_packet >> size;
							shockwave_unit->clear_attacks();
							for (unsigned int i = 0; i < size; ++i) {
								unsigned int attacked_type;
								i_network_packet >> attacked_type;
								if (attacked_type == 0U) {
									unsigned int attacked_id;
									i_network_packet >> attacked_id;
									for (auto& i_own_unit : m_own_units) {
										if (attacked_id == i_own_unit->m_id) {
											shockwave_unit->add_attack(static_cast<Attackable*>(i_own_unit.get()));
										}
									}
								} else if (attacked_type == 1U) {
									unsigned int attacked_x;
									i_network_packet >> attacked_x;
									unsigned int attacked_y;
									i_network_packet >> attacked_y;
									Tile& tile = m_map.get_tile(attacked_x, attacked_y);
									if (BaseTile* base_tile = dynamic_cast<BaseTile*>(&tile)) {
										shockwave_unit->add_attack(static_cast<Attackable*>(base_tile));
									}
								}
							}	
						} else if (WorkerUnit* worker_unit = dynamic_cast<WorkerUnit*>(i_opponent_unit.get())) {
							bool has_exploited;
							i_network_packet >> has_exploited;
							if (has_exploited) {
								unsigned int x;
								i_network_packet >> x;
								unsigned int y;
								i_network_packet >> y;
								Tile& tile = m_map.get_tile(x, y);
								if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(&tile)) {
									worker_unit->start_exploiting(crystal_tile);
								} else if (DestructibleRockTile* destructible_rock_tile = dynamic_cast<DestructibleRockTile*>(&tile)) {
									worker_unit->start_exploiting(destructible_rock_tile);
								}
							} else {
								worker_unit->stop_exploiting();
							}	
						}
					}
				}
			} else if (text == "unit_health") {
				unsigned int id;
				i_network_packet >> id;

				for (auto& i_own_unit : m_own_units) {
					if (i_own_unit->m_id == id) {
						float health;
						i_network_packet >> health;
						i_own_unit->set_health(health);
					}
				}
				for (auto& i_opponent_unit : m_opponent_units) {
					if (i_opponent_unit->m_id == id) {
						float health;
						i_network_packet >> health;
						i_opponent_unit->set_health(health);
					}
				}
			}
		}

		// Update deaths.
	
		for (auto i_own_unit = m_own_units.begin(); i_own_unit != m_own_units.end(); ) {
			if ((*i_own_unit)->is_dead()) {
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
					m_map.set_tile(std::unique_ptr<Tile>(new FloorTile(m_map, i_x, i_y)));
					m_map.update_neighbors_of_tile(i_x, i_y);
					m_smokes.emplace_back(glm::translate(glm::vec3(static_cast<float>(i_x) + 0.5F, 0.2F, static_cast<float>(i_y) + 0.5F)), m_map, 1.2F);
					m_smokes.back().trigger(timer.get_current_time_seconds());
				} else if (BaseTile* base_tile = dynamic_cast<BaseTile*>(&tile)) {
					if (base_tile->is_dead()) {
						glm::vec2 position = static_cast<Attackable*>(base_tile)->get_position_vec2();
						m_map.set_tile(std::unique_ptr<Tile>(new FloorTile(m_map, i_x, i_y)));
						m_explosions.emplace_back(glm::translate(glm::vec3(position.x, 0.2F, position.y)), m_map, 0.75F);
						m_explosions.back().trigger(timer.get_current_time_seconds());
						
						if (!m_is_won) {
							if (base_tile->get_x() == c_own_base_x && base_tile->get_y() == c_own_base_y) { 
								m_won_id = 1;
							} else {
								m_won_id = 0;
							}
							m_is_won = true;
						}
					}
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
		
		// Update smokes.
	
		for (auto i_smoke = m_smokes.begin(); i_smoke != m_smokes.end(); ) {
			if (i_smoke->has_finished()) {
				i_smoke = m_smokes.erase(i_smoke);
			} else {
				i_smoke->update(timer);
				++i_smoke;
			}
		}

		// CLIENT SEND

		if (m_player_id == 0) {
			for (auto& i_own_unit : m_own_units) {
				if (i_own_unit->m_has_changed_path) {
					NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
					network_packet << std::string("move_unit");
					network_packet << i_own_unit->m_id;
					std::list<glm::vec3>& target_path = i_own_unit->get_target_path();
					network_packet << static_cast<unsigned int>(target_path.size());
					for (auto& i_target_position : target_path) {
						network_packet << i_target_position.x;
						network_packet << i_target_position.y;
						network_packet << i_target_position.z;
					}
				
					m_client.get_participant().add_outgoing_network_packet(network_packet);

					i_own_unit->m_has_changed_path = false;
				}
			}
		} else if (m_player_id == 1) {
			for (auto& i_opponent_unit : m_opponent_units) {
				if (i_opponent_unit->m_has_changed_path) {
					NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
					network_packet << std::string("move_unit");
					network_packet << i_opponent_unit->m_id;
					std::list<glm::vec3>& target_path = i_opponent_unit->get_target_path();
					network_packet << static_cast<unsigned int>(target_path.size());
					for (auto& i_target_position : target_path) {
						network_packet << i_target_position.x;
						network_packet << i_target_position.y;
						network_packet << i_target_position.z;
					}
				
					m_client.get_participant().add_outgoing_network_packet(network_packet);

					i_opponent_unit->m_has_changed_path = false;
				}
			}
		}

		if (m_player_id == 0) {
			NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
			network_packet << std::string("select_tiles");
			network_packet << static_cast<unsigned int>(m_own_selected_destructible_rock_tiles.size());
			for (auto& i_tile : m_own_selected_destructible_rock_tiles) {
				network_packet << i_tile->get_x();
				network_packet << i_tile->get_y();
			}
			network_packet << static_cast<unsigned int>(m_own_selected_crystal_tiles.size());
			for (auto& i_tile : m_own_selected_crystal_tiles) {
				network_packet << i_tile->get_x();
				network_packet << i_tile->get_y();
			}
			m_client.get_participant().add_outgoing_network_packet(network_packet);
		} else if (m_player_id == 1) {
			NetworkPacket network_packet = NetworkPacket::create_outgoing(get_network_handler()->m_network_id, NetworkPacket::Type::Update);
			network_packet << std::string("select_tiles");
			network_packet << static_cast<unsigned int>(m_opponent_selected_destructible_rock_tiles.size());
			for (auto& i_tile : m_opponent_selected_destructible_rock_tiles) {
				network_packet << i_tile->get_x();
				network_packet << i_tile->get_y();
			}
			network_packet << static_cast<unsigned int>(m_opponent_selected_crystal_tiles.size());
			for (auto& i_tile : m_opponent_selected_crystal_tiles) {
				network_packet << i_tile->get_x();
				network_packet << i_tile->get_y();
			}
			m_client.get_participant().add_outgoing_network_packet(network_packet);
		}
	} else {
		Networkable::on_update();
	}
}
