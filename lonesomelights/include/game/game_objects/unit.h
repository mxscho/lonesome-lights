#ifndef __GAME__GAME_OBJECTS__UNIT_H__
#define __GAME__GAME_OBJECTS__UNIT_H__

#include "game/game_object.h"

class Timer;

class Unit : public GameObject {
public:
	void update(const Timer& timer) override final;
};

#endif