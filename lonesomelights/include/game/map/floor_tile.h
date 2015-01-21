#ifndef __GAME__MAP__FLOOR_TILE_H__
#define __GAME__MAP__FLOOR_TILE_H__

#include "game/map/tile.h"

#include <glm/glm.hpp>

#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

class Map;
class Timer;

class FloorTile : public Tile {
public:
	FloorTile(const Map& map, unsigned int x, unsigned int y);
	
	void draw(const Camera& camera) const override final;
private:
	VertexBufferObject<glm::vec3> m_vertices_vbo;
	VertexArrayObject m_vertex_array_object;
};

#endif
