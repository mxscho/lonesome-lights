#ifndef __GAME__MAP__MAP_H__
#define __GAME__MAP__MAP_H__

#include "game/map/tile.h"
#include "geometry/transformable.h"
#include "networking/networkable.h"
#include "rendering/drawable.h"
#include "updatable.h"

#include <memory>
#include <vector>
#include <glm/glm.hpp>

class RenderProgram;
class Timer;

class Map : public Drawable, public Networkable, public Updatable, public Transformable {
public:
	Map(const glm::mat4& local_transformation, unsigned int width, unsigned int height);

	Tile& get_tile(unsigned int x, unsigned int y);
	void add_tile(unsigned int x, unsigned int y, std::unique_ptr<Tile>&& tile);
	void delete_tile(unsigned int x, unsigned int y);
	
	void draw(const RenderProgram& render_program) const override final;

	void update(const Timer& timer) override final;
private:
	unsigned int m_width;
	unsigned int m_height;
	std::vector<std::unique_ptr<Tile>> m_tiles;
};

#endif