#ifndef __RENDERING__PARTICLES__PARTICLE_EMITTER_H__
#define __RENDERING__PARTICLES__PARTICLE_EMITTER_H__

#include <list>
#include <glm/glm.hpp>

#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"
#include "rendering/particles/particle.h"
#include "updatable.h"

class Camera;
class RenderProgram;
class Timer;

class ParticleEmitter : public Drawable, public Updatable {
public:
	ParticleEmitter(const glm::vec3& position, const glm::vec3& particle_start_velocity, const glm::vec3& particle_acceleration, float min_particle_lifetime_seconds, float max_particle_lifetime_seconds, float frequency, float current_time_seconds, unsigned int max_particle_count);
	
	void draw(const Camera& camera) const override final;
	void update(const Timer& timer) override final;
private:
	glm::vec3 m_position;
	glm::vec3 m_particle_start_velocity;
	glm::vec3 m_particle_acceleration;
	float m_min_particle_lifetime_seconds;
	float m_max_particle_lifetime_seconds;
	float m_frequency;
	unsigned int m_max_particle_count;
	VertexBufferObject<glm::vec3> m_base_vertex_buffer_object;
	VertexBufferObject<Particle::Data> m_instances_vertex_buffer_object;
	std::list<Particle> m_particles;
	float m_next_emission_time_seconds;
	VertexArrayObject m_vertex_array_object;
};

#endif