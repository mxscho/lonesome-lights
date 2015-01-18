#ifndef __GAME__UNITS__LASER_UNIT_H__
#define __GAME__UNITS__LASER_UNIT_H__

#include "game/units/unit.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"
#include "rendering/particles/particle_systems/laser.h"
#include "rendering/particles/spheric_particle_emitter.h"

#include <list>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Player;

class LaserUnit : public Unit {
public:
	static std::unique_ptr<LaserUnit> create(const glm::vec2& position, const Map& map, const Player& player);

	void start_shooting(const glm::vec2& position);
	void stop_shooting();
	
	void draw(const Camera& camera) const override final;
	
	void update(const Timer& timer) override final;
private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec3& normal);
		
		glm::vec3 position;
		glm::vec3 normal;
	};
	
	LaserUnit(const glm::vec2& position, const Map& map, const Player& player, const std::vector<Data>& vertices, const std::vector<unsigned int>& vertex_counts);

	VertexBufferObject<Data> m_vertices_vbo;
	std::vector<unsigned int> m_vertex_counts;
	VertexBufferObject<GLuint> m_vine_elements_vbo;
	VertexBufferObject<GLuint> m_ball_elements_vbo;
	VertexArrayObject m_vine_vao;
	VertexArrayObject m_ball_vao;
	glm::mat4 m_vine_transformation;
	glm::mat4 m_ball_transformation;
	
	Laser m_laser;
	SphericParticleEmitter m_flames;
};

#endif