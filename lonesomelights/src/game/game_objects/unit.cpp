#include "game/game_objects/unit.h"

void Unit::update(const Timer& timer) {
	GameObject::update(timer);
	
	// TODO: Update unit.
	
	// TEST
	Networkable::on_update();
}