#include "game/map/map.h"

#include "game/map/base_tile.h"
#include "game/map/crystal_tile.h"
#include "game/map/destructible_rock_tile.h"
#include "game/map/floor_tile.h"
#include "game/map/indestructible_rock_tile.h"
#include "game/map/rock_tile.h"

#include <glm/glm.hpp>
#include <fstream>
#include <string>

Map Map::create_empty_map(unsigned int tile_count_x, unsigned int tile_count_y, float tile_size) {
	Map map(tile_count_x, tile_count_y, tile_size);
	for (unsigned int i_y = 0; i_y < map.get_tile_count_y(); ++i_y) {
		for (unsigned int i_x = 0; i_x < map.get_tile_count_x(); ++i_x) {
			map.set_tile(std::unique_ptr<Tile>(new FloorTile(map, i_x, i_y)));
		}
	}
	
	return map;
}

Map Map::create_map(float tile_size) {
	unsigned int map_size = 30;
	Map map = create_empty_map(map_size, map_size, tile_size);
	
	std::string line;
	std::ifstream map_file ("res/maps/default_map.txt");
	unsigned int y = 29;
	if (map_file.is_open()) {
		while (getline(map_file, line)) {
			for (unsigned int x = 0; x < map_size; ++x) {
				if (line[x] == 'X') {
					map.set_tile(std::unique_ptr<Tile>(new IndestructibleRockTile(map, y, x, RockTile::CliffType::None)));
				} else if (line[x] == 'D') {
					map.set_tile(std::unique_ptr<Tile>(new DestructibleRockTile(map, y, x, RockTile::CliffType::None)));
				} else if (line[x] == 'C') {
					map.set_tile(std::unique_ptr<Tile>(new CrystalTile(CrystalTile::create(map, y, x, RockTile::CliffType::None))));
				}
			}
			y--;
		}
		map_file.close();
	}
	
	for (unsigned int i_x = 0; i_x < map_size; ++i_x) {
		for (unsigned int i_y = 0; i_y < map_size; ++i_y) {
			map.update_tile(i_x, i_y);
		}	
	}
	
	for (unsigned int i_y = 0; i_y < map.get_tile_count_y(); ++i_y) {
		for (unsigned int i_x = 0; i_x < map.get_tile_count_x(); ++i_x) {
			assert(!!map.m_tiles[i_y * map.m_tile_count_x + i_x]);
		}
	}

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

void Map::set_floor_tile(unsigned int x, unsigned int y) {
}
void Map::set_indestructible_rock_tile(unsigned int x, unsigned int y) {
}
void Map::set_destructible_rock_tile(unsigned int x, unsigned int y) {
}
void Map::set_crystal_tile(unsigned int x, unsigned int y) {
}
void Map::set_base_tile(unsigned int x, unsigned int y, Player& player) {
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
	for (auto& i_tile : m_tiles) {
		if (Attackable* attackable = dynamic_cast<Attackable*>(i_tile.get())) {
			attackable->draw(camera);
		}
	}
}

void Map::draw_extras(const Camera& camera) const {
	for (auto& i_tile : m_tiles) {
		if (!!i_tile) {
			if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(i_tile.get())) {
				crystal_tile->draw_crystals(camera);
			}
			if (BaseTile* base_tile = dynamic_cast<BaseTile*>(i_tile.get())) {
				base_tile->draw_flames(camera);
			}
		}
	}
}

void Map::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	for (auto& i_tile : m_tiles) {
		if (!!i_tile) {
			if (BaseTile* base_tile = dynamic_cast<BaseTile*>(i_tile.get())) {
				base_tile->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
			} else if (CrystalTile* crystal_tile = dynamic_cast<CrystalTile*>(i_tile.get())) {
				crystal_tile->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
			}
		}
	}
}

void Map::update(const Timer& timer) {
	Updatable::update(timer);
	
	for (auto& i_tile : m_tiles) {
		if (!!i_tile) {
			i_tile->update(timer);
		}
	}
	
	// TEST
	Networkable::on_update();
}

void Map::set_tile(std::unique_ptr<Tile>&& tile) {
	unsigned int x = tile->get_x();
	unsigned int y = tile->get_y();
	delete_tile(x, y);

	m_tiles[y * m_tile_count_x + x] = std::move(tile);
}

void Map::update_tile(unsigned int x, unsigned int y) {

	unsigned short cliff_type = 0;
	if (y + 1 < m_tile_count_y) {
		if (!m_tiles[(y + 1) * m_tile_count_x + x]->is_rock()) {
			cliff_type += 4;
		}
	}
	if (y > 0) {
		if (!m_tiles[(y - 1) * m_tile_count_x + x]->is_rock()) {
			cliff_type += 8;
		}
	}
	if (x + 1 < m_tile_count_x) {
		if (!m_tiles[y * m_tile_count_x + x + 1]->is_rock()) {
			cliff_type += 1;
		}
	}
	if (x > 0) {
		if (!m_tiles[y * m_tile_count_x + x - 1]->is_rock()) {
			cliff_type += 2;
		}
	}
	if (y == 0) {
		cliff_type += 8;
	}
	if (y == m_tile_count_y - 1) {
		cliff_type += 4;
	}
	if (x == 0) {
		cliff_type += 2;
	}
	if (x == m_tile_count_x - 1) {
		cliff_type += 1;
	}
	m_tiles[y * m_tile_count_x + x]->set_cliff_type(cliff_type);
}

void Map::update_neighbors_of_tile(unsigned int x, unsigned int y) {
	if (y + 1 < m_tile_count_y) {
		update_tile(x, y + 1);
	}
	if (y > 0) {
		update_tile(x, y - 1);
	}
	if (x + 1 < m_tile_count_x) {
		update_tile(x + 1, y);
	}
	if (x > 0) {
		update_tile(x - 1, y);
	}
}
