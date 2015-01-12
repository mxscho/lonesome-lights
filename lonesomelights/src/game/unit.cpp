#include "game/unit.h"

void Unit::draw(const RenderProgram& render_program, const Camera& camera) const {
	Drawable::draw(render_program, camera);
	Drawable::prepare_draw(render_program, camera);
	
	// TODO: Draw unit.
	
	Drawable::finalize_draw(render_program);
}

void Unit::update(const Timer& timer) {
	Updatable::update(timer);
	
	// TODO: Update unit.
	
	// TEST
	Networkable::on_update();
}