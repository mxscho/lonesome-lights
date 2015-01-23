#ifndef __GAME__GAME_H__
#define __GAME__GAME_H__

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
	Game();
	
	Map& get_map();
	std::list<std::unique_ptr<Unit>>& get_own_units();
	std::list<std::unique_ptr<Unit>>& get_opponent_units();
	
	void draw(const Camera& camera) const;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const;
	
	void update(const Timer& timer) override final;
private:
	Map m_map;
	Player m_own_player;
	Player m_opponent_player;
	std::list<std::unique_ptr<Unit>> m_own_units;
	std::list<std::unique_ptr<Unit>> m_opponent_units;
	std::list<Explosion> m_explosions;
};

#endif
