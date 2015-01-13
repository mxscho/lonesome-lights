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
	Tile(const Map& map, unsigned int x, unsigned int y);

	void draw(const Camera& camera) const override final;
private:
	VertexBufferObject<glm::vec3> m_vertices_vbo;
	VertexArrayObject m_vertex_array_object;
};

#endif