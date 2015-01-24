#ifndef __GAME__MAP__CRYSTAL_TILE_H__
#define __GAME__MAP__CRYSTAL_TILE_H__

#include "game/map/floor_tile.h"
#include "game/map/rock_tile.h"
#include "game/map/tile.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

#include <glm/glm.hpp>
#include <vector>

class Map;
class Timer;
class Player;

class CrystalTile : public Tile {
public:

	static CrystalTile create(const Map& map, unsigned int x, unsigned int y, const RockTile::CliffType& cliff_type);
	
	void draw(const Camera& camera) const override final;
private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec3& normal);
		
		glm::vec3 position;
		glm::vec3 normal;
	};
	
	CrystalTile(const Map& map, unsigned int x, unsigned int y, const std::vector<Data>& vertices, const RockTile::CliffType& cliff_type);	
	
	VertexBufferObject<Data> m_vertices_vbo;
	VertexBufferObject<GLuint> m_crystals_elements_vbo;
	VertexArrayObject m_crystals_vao;
	RockTile m_rock_tile;
	glm::mat4 m_crystal_tile_transformation;
};

#endif
