#ifndef __GAME__GAME_OBJECT_H__
#define __GAME__GAME_OBJECT_H__

#include "networking/networkable.h"
#include "updatable.h"

class Timer;

class GameObject: public Networkable, public Updatable {
};

#endif