#ifndef __GAME__MAP__BASE_TILE_H__
#define __GAME__MAP__BASE_TILE_H__

#include "game/map/tile.h"
#include "game/map/floor_tile.h"

#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

#include <glm/glm.hpp>
#include <vector>

class Map;
class Timer;
class Player;

class BaseTile : public Tile {
public:

	static BaseTile create(const Map& map, unsigned int x, unsigned int y);
	
	void draw(const Camera& camera) const override final;
private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec3& normal);
		
		glm::vec3 position;
		glm::vec3 normal;
	};
	
	BaseTile(const Map& map, unsigned int x, unsigned int y, const std::vector<Data>& vertices);
	
	VertexBufferObject<Data> m_vertices_vbo;
	VertexBufferObject<GLuint> m_elements_vbo;
	VertexArrayObject m_vertex_array_object;
	FloorTile m_floor_tile;
	glm::mat4 m_base_transformation;
};

#endif
