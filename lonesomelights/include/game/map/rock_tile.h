#ifndef __GAME__MAP__ROCK_TILE_H__
#define __GAME__MAP__ROCK_TILE_H__

#include "game/map/tile.h"

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

class Map;
class Timer;

class RockTile : public Tile {
public:
	enum class CliffType : unsigned short {
		None = 0,
		
		PositiveX = 1,
		NegativeX = 2,
		PositiveY = 4,
		NegativeY = 8,
		
		PositiveXNegativeX = 1 + 2,
		PositiveXPositiveY = 1 + 4,
		PositiveXNegativeY = 1 + 8,
		NegativeXPositiveY = 2 + 4,
		NegativeXNegativeY = 2 + 8,
		PositiveYNegativeY = 4 + 8,
		
		PositiveXNegativeXPositiveY = 1 + 2 + 4,
		PositiveXNegativeXNegativeY = 1 + 2 + 8,
		PositiveXPositiveYNegativeY = 1 + 4 + 8,
		NegativeXPositiveYNegativeY = 2 + 4 + 8,
		
		PositiveXNegativeXPositiveYNegativeY = 1 + 2 + 4 + 8
	};

	static RockTile create(const Map& map, unsigned int x, unsigned int y, const CliffType& cliff_type);
	
	void draw(const Camera& camera) const override final;
private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texel);
		
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texel;
	};
	
	RockTile(const Map& map, unsigned int x, unsigned int y, const CliffType& cliff_type, const std::vector<RockTile::Data>& cliff_vertices, const std::vector<RockTile::Data>& floor_vertices, const std::vector<GLushort>& floor_elements);

	CliffType m_cliff_type;
	VertexBufferObject<Data> m_cliff_vertices_vbo;
	VertexArrayObject m_cliff_vao;
	VertexBufferObject<Data> m_floor_vertices_vbo;
	VertexBufferObject<GLushort> m_floor_elements_vbo;
	VertexArrayObject m_floor_vao;
};

#endif
