#ifndef __GAME__MAP__BASE_TILE_H__
#define __GAME__MAP__BASE_TILE_H__

#include "game/attackable.h"
#include "game/map/tile.h"
#include "game/map/floor_tile.h"
#include "game/player.h"
#include "rendering/opengl/texture.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"
#include "rendering/particles/spheric_particle_emitter.h"

#include <glm/glm.hpp>
#include <vector>

class Map;
class Timer;
class Player;

class BaseTile : public Attackable, public Tile {
public:
	static BaseTile create(const Map& map, unsigned int x, unsigned int y, const Player& player);
	
	glm::vec2 get_position_vec2() const override final;

	const Player& get_player() const;

	void update(const Timer& timer) override final;

	void draw(const Camera& camera) const override final;
	void draw_flames(const Camera& camera) const;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const;
	bool is_rock() const override final;
	void set_cliff_type(unsigned short cliff_type) override final;
private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec3& normal);
		
		glm::vec3 position;
		glm::vec3 normal;
	};
	
	BaseTile(const Map& map, unsigned int x, unsigned int y, const std::vector<Data>& vertices, const Player& player);

	VertexBufferObject<Data> m_vertices_vbo;
	VertexBufferObject<GLuint> m_elements_vbo;
	VertexArrayObject m_vertex_array_object;
	FloorTile m_floor_tile;
	glm::mat4 m_base_transformation;

	const Player& m_player;
	SphericParticleEmitter m_flames1;
	SphericParticleEmitter m_flames2;
	SphericParticleEmitter m_flames3;
	SphericParticleEmitter m_flames4;
	SphericParticleEmitter m_flames5;
};

#endif
