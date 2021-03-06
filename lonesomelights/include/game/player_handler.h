#ifndef __GAME__PLAYER_HANDLER_H__
#define __GAME__PLAYER_HANDLER_H__

#include "game/client_game.h"
#include "game/map/destructible_rock_tile.h"
#include "game/map/crystal_tile.h"
#include "game/units/unit.h"
#include "geometry/path_finder.h"
#include "updatable.h"

#include <glm/glm.hpp>

class Timer;

class PlayerHandler : public Updatable {
public:
	PlayerHandler(ClientGame& game);
	
	void update(const Timer& timer) override final;
	
	void on_mouse_hover(const Timer& timer, const glm::vec3& position);
	void on_mouse_select(const Timer& timer, const glm::vec3& position, bool is_left, bool is_shift);
private:
	void check_selected_unit();

	ClientGame& m_game;
	PathFinder m_path_finder;
	Unit* m_selected_unit;
};

#endif
