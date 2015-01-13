#ifndef __GAME__MAP__MAP_H__
#define __GAME__MAP__MAP_H__

#include "game/map/tile.h"
#include "geometry/transformable.h"
#include "networking/networkable.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"
#include "updatable.h"

#include <memory>
#include <vector>
#include <GL/glew.h>

class Camera;
class RenderProgram;
class Timer;

class Map : public Drawable, public Networkable, public Updatable, public Transformable {
public:
	Map(unsigned int tile_count_x, unsigned int tile_count_y, float tile_size);

	unsigned int get_tile_count_x() const;
	unsigned int get_tile_count_y() const;
	float get_tile_size() const;
	Tile& get_tile(unsigned int x, unsigned int y);
	void set_tile(std::unique_ptr<Tile>&& tile);
	void delete_tile(unsigned int x, unsigned int y);
	
	void draw(const Camera& camera) const override final;

	void update(const Timer& timer) override final;
private:
	unsigned int m_tile_count_x;
	unsigned int m_tile_count_y;
	float m_tile_size;
	std::vector<std::unique_ptr<Tile>> m_tiles;
	VertexBufferObject<glm::vec3> m_vertices_vbo;
	VertexBufferObject<GLushort> m_elements_vbo;
	VertexArrayObject m_vertex_array_object;
};

#endif