#ifndef __GAME__SERVER_GAME_H__
#define __GAME__SERVER_GAME_H__

#include "game/map/destructible_rock_tile.h"
#include "game/map/crystal_tile.h"
#include "game/map/map.h"
#include "game/player.h"
#include "game/units/unit.h"
#include "networking/server.h"
#include "networking/networkable.h"
#include "networking/network_packet.h"
#include "networking/network_handlers/server_handlers/game_server_handler.h"
#include "rendering/particles/particle_systems/explosion.h"
#include "updatable.h"

#include <memory>
#include <list>

class Camera;
class Texture;
class Timer;

class ServerGame : public Networkable, public Updatable {
friend class GameServerHandler;
friend class GameClientHandler;
public:
	static float c_worker_unit_plasma_cost;
	static float c_worker_unit_crystals_cost;
	static float c_laser_unit_plasma_cost;
	static float c_laser_unit_crystals_cost;
	static float c_shockwave_unit_plasma_cost;
	static float c_shockwave_unit_crystals_cost;

	static float c_upgrade1_plasma_cost;
	static float c_upgrade1_crystals_cost;
	static float c_upgrade2_plasma_cost;
	static float c_upgrade2_crystals_cost;

	static float c_plasma_generation;
	static float c_crystals_generation;

	static unsigned int c_own_base_x;
	static unsigned int c_own_base_y;
	static unsigned int c_opponent_base_x;
	static unsigned int c_opponent_base_y;

	ServerGame(Server& server);

	bool has_started() const;
	void start();

	float get_own_plasma_count() const;
	float get_own_crystal_count() const;
	
	Map& get_map();
	std::list<std::unique_ptr<Unit>>& get_own_units();
	std::list<std::unique_ptr<Unit>>& get_opponent_units();

	void spawn_own_worker_unit();
	void spawn_own_laser_unit();
	void spawn_own_shockwave_unit();
	void spawn_opponent_worker_unit();
	void spawn_opponent_laser_unit();
	void spawn_opponent_shockwave_unit();
	
	void update(const Timer& timer) override final;

	std::vector<DestructibleRockTile*> m_own_selected_destructible_rock_tiles;
	std::vector<CrystalTile*> m_own_selected_crystal_tiles;
	std::vector<DestructibleRockTile*> m_opponent_selected_destructible_rock_tiles;
	std::vector<CrystalTile*> m_opponent_selected_crystal_tiles;
private:
	unsigned int m_current_unit_id;

	bool m_is_started;
	Map m_map;
	Player m_own_player;
	Player m_opponent_player;
	std::list<std::unique_ptr<Unit>> m_own_units;
	std::list<std::unique_ptr<Unit>> m_opponent_units;
	float m_own_plasma_count;
	float m_own_crystal_count;
	float m_opponent_plasma_count;
	float m_opponent_crystal_count;

	Server& m_server;
	unsigned int m_player_id;
};

#endif
