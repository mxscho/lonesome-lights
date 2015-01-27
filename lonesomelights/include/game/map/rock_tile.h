#ifndef __GAME__MAP__ROCK_TILE_H__
#define __GAME__MAP__ROCK_TILE_H__

#include "game/map/tile.h"

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game/map/floor_tile.h"
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
	
	void draw(const Camera& camera) const override;
	void set_color(glm::vec3 new_color);
	void set_cliff_type(unsigned short cliff_type) override final;
	CliffType get_cliff_type() const;
	bool is_rock() const override final;
private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texel);
		
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texel;
	};
	
	RockTile(const Map& map, unsigned int x, unsigned int y, const CliffType& cliff_type, const std::vector<RockTile::Data>& cliff_vertices, const std::vector<RockTile::Data>& floor_vertices);

	CliffType m_cliff_type;
	VertexBufferObject<Data> m_cliff_vertices_vbo;
	VertexArrayObject m_cliff_vao;
	VertexBufferObject<Data> m_floor_vertices_vbo;
	VertexArrayObject m_floor_vao;
	FloorTile m_floor_tile;
	glm::vec4 m_color;
};

#endif
