#ifndef __GAME__PLAYER_HANDLER_H__
#define __GAME__PLAYER_HANDLER_H__

#include "game/game.h"
#include "game/units/unit.h"
#include "updatable.h"

#include <glm/glm.hpp>

class Timer;

class PlayerHandler : public Updatable {
public:
	PlayerHandler(Game& game);
	
	void update(const Timer& timer) override final;
	
	void on_mouse_select(const glm::vec3& position, bool is_left);
private:
	Game& m_game;
	Unit* m_selected_unit;
};

#endif