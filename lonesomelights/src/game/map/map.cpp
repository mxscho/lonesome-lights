#include "game/map/map.h"

Map::Map(const glm::mat4& local_transformation, unsigned int width, unsigned int height)
	: Drawable(),
	Networkable(),
	Updatable(),
	Transformable(local_transformation),
	m_width(width),
	m_height(height),
	m_tiles(m_width * m_height) {
}

Tile& Map::get_tile(unsigned int x, unsigned int y) {
	return *m_tiles[y * m_width + x];
}
void Map::add_tile(unsigned int x, unsigned int y, std::unique_ptr<Tile>&& tile) {
	delete_tile(x, y);
	m_tiles[y * m_width + x] = std::move(tile);
}
void Map::delete_tile(unsigned int x, unsigned int y) {
	m_tiles[y * m_width + x].reset();
}

void Map::draw(const RenderProgram& render_program) const {
	Drawable::draw(render_program);
	
	// TODO: Draw map.
}

void Map::update(const Timer& timer) {
	Updatable::update(timer);
	
	// TODO: Update map.
	
	// TEST
	Networkable::on_update();
}