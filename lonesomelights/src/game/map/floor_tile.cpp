#include "game/map/floor_tile.h"

void FloorTile::draw(const RenderProgram& render_program, const Camera& camera) const {
	Drawable::draw(render_program, camera);
	Drawable::prepare_draw(render_program, camera);
	
	// TODO: Draw floor tile.
	
	Drawable::finalize_draw(render_program);
}