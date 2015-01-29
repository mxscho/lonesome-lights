#ifndef __GAME__MAP__CRYSTAL_TILE_H__
#define __GAME__MAP__CRYSTAL_TILE_H__

#include "game/attackable.h"
#include "game/map/floor_tile.h"
#include "game/map/destructible_rock_tile.h"
#include "game/map/tile.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"
#include "rendering/particles/spheric_particle_emitter.h"

#include <glm/glm.hpp>
#include <vector>

class Map;
class Timer;
class Player;

class CrystalTile : public Attackable, public Tile {
public:

	static CrystalTile create(const Map& map, unsigned int x, unsigned int y, const RockTile::CliffType& cliff_type);
	
	glm::vec2 get_position_vec2() const override final;

	void draw(const Camera& camera) const override final;
	void draw_crystals(const Camera& camera) const;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const;

	void update(const Timer& timer) override final;

	bool is_selected() const;
	void unselect();
	void select(const glm::vec3& color);
	void unhover();
	void hover(const glm::vec3& color);
	
	void set_cliff_type(unsigned short cliff_type) override final;
	RockTile::CliffType get_cliff_type() const;
	bool is_rock() const override final;

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
	DestructibleRockTile m_destructible_rock_tile;
	glm::mat4 m_crystal_tile_transformation;
	SphericParticleEmitter m_particles;
};

#endif
