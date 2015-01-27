#ifndef __GAME__UNITS__SHOCKWAVE_UNIT_H__
#define __GAME__UNITS__SHOCKWAVE_UNIT_H__

#include "game/attackable.h"
#include "game/units/unit.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"
#include "game/units/shockwave.h"
#include "rendering/particles/spheric_particle_emitter.h"

#include <set>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Player;

class ShockwaveUnit : public Drawable, public Unit {
public:
	static std::unique_ptr<ShockwaveUnit> create(const glm::vec2& position, const Map& map, const Player& player);

	float get_attack_range() const override final;
	float get_attack_dps() const override final;
	
	const std::set<Attackable*>& get_attacked() const;
	void add_attack(Attackable* attackable);
	void remove_attack(Attackable* attackable);
	void clear_attacks();
	
	void draw(const Camera& camera) const override final;
	void draw_shockwave(const Camera& camera) const;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const;
	
	void update(const Timer& timer) override final;
private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec3& normal);
		
		glm::vec3 position;
		glm::vec3 normal;
	};
	
	ShockwaveUnit(const glm::vec2& position, const Map& map, const Player& player, const std::vector<Data>& vertices, const std::vector<unsigned int>& vertex_counts);

	VertexBufferObject<Data> m_vertices_vbo;
	std::vector<unsigned int> m_vertex_counts;
	VertexBufferObject<GLuint> m_vine_elements_vbo;
	VertexBufferObject<GLuint> m_ball_elements_vbo;
	VertexArrayObject m_vine_vao;
	VertexArrayObject m_ball_vao;
	glm::mat4 m_vine_transformation;
	glm::mat4 m_ball_transformation;
	
	Shockwave m_shockwave;
	std::set<Attackable*> m_attacked;
	
	SphericParticleEmitter m_flash;
};

#endif
