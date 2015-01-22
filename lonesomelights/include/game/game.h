#ifndef __GAME__GAME_H__
#define __GAME__GAME_H__

#include "game/map/map.h"
#include "game/player.h"
#include "game/units/unit.h"
#include "networking/networkable.h"
#include "updatable.h"

#include <memory>
#include <vector>

class Camera;
class Texture;
class Timer;

class Game : public Networkable, public Updatable {
public:
	Game();
	
	Map& get_map();
	std::vector<std::unique_ptr<Unit>>& get_own_units();
	std::vector<std::unique_ptr<Unit>>& get_opponent_units();
	
	void draw(const Camera& camera) const;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const;
	
	void update(const Timer& timer) override final;
private:
	Map m_map;
	Player m_own_player;
	Player m_opponent_player;
	std::vector<std::unique_ptr<Unit>> m_own_units;
	std::vector<std::unique_ptr<Unit>> m_opponent_units;
};

#endif
