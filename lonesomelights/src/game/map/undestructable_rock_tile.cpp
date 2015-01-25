#include "game/map/undestructable_rock_tile.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "game/map/map.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"


UndestructableRockTile::UndestructableRockTile(const Map& map, unsigned int x, unsigned int y, const CliffType& cliff_type)
	: RockTile(RockTile::create(map, x, y, cliff_type)),
	m_standard_color(0.2f, 0.2f, 0.2f) {
	
	RockTile::set_color(m_standard_color);
}

void UndestructableRockTile::draw(const Camera& camera) const {
	RockTile::draw(camera);
}

