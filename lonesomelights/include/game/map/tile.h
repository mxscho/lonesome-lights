#ifndef __GAME__MAP__TILE_H__
#define __GAME__MAP__TILE_H__

#include "geometry/transformable.h"
#include "networking/networkable.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

class Map;
class RenderProgram;
class Timer;

class Tile : public Drawable, public Networkable, public Transformable {
public:
	Tile(const Map& map, unsigned int x, unsigned int y, const RenderProgram& render_program);

	unsigned int get_x() const;
	unsigned int get_y() const;
	float get_size() const;
	virtual bool is_walkable() const;
private:
	const Map& m_map;
	unsigned int m_x;
	unsigned int m_y;
};

#endif