#ifndef __GAME__MAP__TILE_H__
#define __GAME__MAP__TILE_H__

#include "geometry/transformable.h"
#include "networking/networkable.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"
#include "updatable.h"

class Map;
class RenderProgram;
class Timer;

class Tile : public Drawable, public Networkable, public Transformable, public Updatable {
public:
	Tile(const Map& map, unsigned int x, unsigned int y, const RenderProgram& render_program);
	virtual ~Tile();

	unsigned int get_x() const;
	unsigned int get_y() const;
	float get_size() const;
	bool is_walkable() const;
	void set_is_walkable(bool walkable);

	void update(const Timer& timer) override;
private:
	const Map& m_map;
	unsigned int m_x;
	unsigned int m_y;
	bool m_walkable;
};

#endif
