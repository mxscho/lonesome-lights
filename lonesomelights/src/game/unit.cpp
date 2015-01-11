#include "game/unit.h"

void Unit::draw(const RenderProgram& render_program) const {
	Drawable::draw(render_program);
	
	// TODO: Draw unit.
}

void Unit::update(const Timer& timer) {
	Updatable::update(timer);
	
	// TODO: Update unit.
	
	// TEST
	Networkable::on_update();
}