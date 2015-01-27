#include "game/map/tile.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "game/map/map.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"

//test
#include <iostream>

Tile::Tile(const Map& map, unsigned int x, unsigned int y, const RenderProgram& render_program)
	: Drawable(render_program),
	Networkable(),
	Transformable(glm::translate(glm::vec3((float) x, 0.0F, (float) y)), map),
	m_map(map),
	m_x(x),
	m_y(y),
	m_walkable(true) {
}
Tile::~Tile() {
}

unsigned int Tile::get_x() const {
	return m_x;
}
unsigned int Tile::get_y() const {
	return m_y;
}
float Tile::get_size() const {
	return m_map.get_tile_size();
}
bool Tile::is_walkable() const {
	return m_walkable;
}

void Tile::set_is_walkable(bool walkable) {
	m_walkable = walkable;
}

void Tile::update(const Timer& timer) {
}
