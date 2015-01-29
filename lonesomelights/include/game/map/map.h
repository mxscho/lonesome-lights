#ifndef __GAME__MAP__MAP_H__
#define __GAME__MAP__MAP_H__

#include "game/map/tile.h"
#include "geometry/transformable.h"
#include "networking/networkable.h"
#include "rendering/opengl/texture.h"
#include "updatable.h"

#include <memory>
#include <vector>

class Camera;
class Player;
class RenderProgram;
class Timer;

class Map : public Networkable, public Updatable, public Transformable {
public:
	static Map create_empty_map(unsigned int tile_count_x, unsigned int tile_count_y, float tile_size);
	static Map create_map(float tile_size);

	Map(unsigned int tile_count_x, unsigned int tile_count_y, float tile_size);

	unsigned int get_tile_count_x() const;
	unsigned int get_tile_count_y() const;
	float get_tile_size() const;
	Tile& get_tile(unsigned int x, unsigned int y);
	void delete_tile(unsigned int x, unsigned int y);
	void set_floor_tile(unsigned int x, unsigned int y);
	void set_indestructible_rock_tile(unsigned int x, unsigned int y);
	void set_destructible_rock_tile(unsigned int x, unsigned int y);
	void set_crystal_tile(unsigned int x, unsigned int y);
	void set_base_tile(unsigned int x, unsigned int y, Player& player);
	void draw(const Camera& camera) const;
	void draw_extras(const Camera& camera) const;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const;

	void update(const Timer& timer) override final;
	
	void set_tile(std::unique_ptr<Tile>&& tile);
	void update_tile(unsigned int x, unsigned int y);
	void update_neighbors_of_tile(unsigned int x, unsigned int y);
private:
	unsigned int m_tile_count_x;
	unsigned int m_tile_count_y;
	float m_tile_size;
	std::vector<std::unique_ptr<Tile>> m_tiles;
};

#endif
