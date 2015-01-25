#include "game/map/map.h"

#include "game/map/crystal_tile.h"
#include "game/map/destructable_rock_tile.h"
#include "game/map/floor_tile.h"
#include "game/map/rock_tile.h"
#include "game/map/undestructable_rock_tile.h"

#include <glm/glm.hpp>


//test
#include <iostream>

Map Map::create_empty_map(unsigned int tile_count_x, unsigned int tile_count_y, float tile_size) {
	Map map(tile_count_x, tile_count_y, tile_size);
	for (unsigned int i_y = 0; i_y < map.get_tile_count_y(); ++i_y) {
		for (unsigned int i_x = 0; i_x < map.get_tile_count_x(); ++i_x) {
			map.set_tile(std::unique_ptr<Tile>(new FloorTile(map, i_x, i_y)));
		}
	}
	
	return map;
}

Map Map::create_test_map(float tile_size) {
	Map map = create_empty_map(20, 20, tile_size);
	map.set_tile(std::unique_ptr<Tile>(new CrystalTile(CrystalTile::create(map, 9, 9, RockTile::CliffType::NegativeXNegativeY))));
	map.set_tile(std::unique_ptr<Tile>(new CrystalTile(CrystalTile::create(map, 9, 10, RockTile::CliffType::NegativeX))));
	map.set_tile(std::unique_ptr<Tile>(new CrystalTile(CrystalTile::create(map, 9, 11, RockTile::CliffType::NegativeXPositiveY))));
	map.set_tile(std::unique_ptr<Tile>(new DestructableRockTile(map, 10, 9, RockTile::CliffType::NegativeY)));
	map.set_tile(std::unique_ptr<Tile>(new UndestructableRockTile(map, 10, 10, RockTile::CliffType::None)));
	map.set_tile(std::unique_ptr<Tile>(new UndestructableRockTile(map, 10, 11, RockTile::CliffType::PositiveY)));
	map.set_tile(std::unique_ptr<Tile>(new DestructableRockTile(map, 11, 9, RockTile::CliffType::PositiveXNegativeY)));
	map.set_tile(std::unique_ptr<Tile>(new DestructableRockTile(map, 11, 10, RockTile::CliffType::PositiveX)));
	map.set_tile(std::unique_ptr<Tile>(new DestructableRockTile(map, 11, 11, RockTile::CliffType::PositiveXPositiveY)));
	
	map.set_tile(std::unique_ptr<Tile>(new CrystalTile(CrystalTile::create(map, 13, 11, RockTile::CliffType::NegativeXPositiveYNegativeY))));
	map.set_tile(std::unique_ptr<Tile>(new DestructableRockTile(map, 14, 11, RockTile::CliffType::PositiveXPositiveYNegativeY)));
	map.set_tile(std::unique_ptr<Tile>(new CrystalTile(CrystalTile::create(map, 1, 3, RockTile::CliffType::NegativeXNegativeY))));
	map.set_tile(std::unique_ptr<Tile>(new DestructableRockTile(map, 1, 4, RockTile::CliffType::NegativeXNegativeY)));
	map.set_tile(std::unique_ptr<Tile>(new UndestructableRockTile(map, 0, 4, RockTile::CliffType::NegativeXNegativeY)));
	return map;
}

Map::Map(unsigned int tile_count_x, unsigned int tile_count_y, float tile_size)
	: Networkable(),
	Updatable(),
	Transformable(glm::mat4()),
	m_tile_count_x(tile_count_x),
	m_tile_count_y(tile_count_y),
	m_tile_size(tile_size),
	m_tiles(m_tile_count_x * m_tile_count_y) {	
}

unsigned int Map::get_tile_count_x() const {
	return m_tile_count_x;
}
unsigned int Map::get_tile_count_y() const {
	return m_tile_count_y;
}
float Map::get_tile_size() const {
	return m_tile_size;
}
Tile& Map::get_tile(unsigned int x, unsigned int y) {
	assert(!!m_tiles[y * m_tile_count_x + x]);
	return *m_tiles[y * m_tile_count_x + x];
}
void Map::set_tile(std::unique_ptr<Tile>&& tile) {
	unsigned int x = tile->get_x();
	unsigned int y = tile->get_y();
	delete_tile(x, y);
	m_tiles[y * m_tile_count_x + x] = std::move(tile);
}
void Map::delete_tile(unsigned int x, unsigned int y) {
	m_tiles[y * m_tile_count_x + x].reset();
}

void Map::draw(const Camera& camera) const {
	for (auto& i_tile : m_tiles) {
		if (!!i_tile) {
			i_tile->draw(camera);
		}
	}
}

void Map::draw_extras(const Camera& camera) const {
	for (auto& i_tile : m_tiles) {
		if (!!i_tile) {
			if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(i_tile.get())) {
				crystal_tile->draw_crystals(camera);
			}
		}
	}
}

void Map::update(const Timer& timer) {
	Updatable::update(timer);
	
	// TODO: Update map.
	
	// TEST
	Networkable::on_update();
}
