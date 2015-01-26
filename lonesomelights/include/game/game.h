#ifndef __GAME__GAME_H__
#define __GAME__GAME_H__

#include "game/map/destructible_rock_tile.h"
#include "game/map/crystal_tile.h"
#include "game/map/map.h"
#include "game/player.h"
#include "game/units/unit.h"
#include "networking/networkable.h"
#include "rendering/particles/particle_systems/explosion.h"
#include "updatable.h"

#include <memory>
#include <list>

class Camera;
class Texture;
class Timer;

class Game : public Networkable, public Updatable {
public:
	static float c_worker_unit_plasma_cost;
	static float c_worker_unit_crystals_cost;
	static float c_laser_unit_plasma_cost;
	static float c_laser_unit_crystals_cost;
	static float c_shockwave_unit_plasma_cost;
	static float c_shockwave_unit_crystals_cost;

	static unsigned int c_own_base_x;
	static unsigned int c_own_base_y;
	static unsigned int c_opponent_base_x;
	static unsigned int c_opponent_base_y;

	Game();

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

	void draw(const Camera& camera) const;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const;
	
	void update(const Timer& timer) override final;

	std::vector<DestructibleRockTile*> m_own_selected_destructible_rock_tiles;
	std::vector<CrystalTile*> m_own_selected_crystal_tiles;
	std::vector<DestructibleRockTile*> m_opponent_selected_destructible_rock_tiles;
	std::vector<CrystalTile*> m_opponent_selected_crystal_tiles;
private:
	bool m_is_started;
	Map m_map;
	Player m_own_player;
	Player m_opponent_player;
	std::list<std::unique_ptr<Unit>> m_own_units;
	std::list<std::unique_ptr<Unit>> m_opponent_units;
	float m_own_plasma_count;
	float m_own_crystal_count;
	//float m_opponent_plasma_count;
	float m_opponent_crystal_count;

	std::list<Explosion> m_explosions;
};

#endif
