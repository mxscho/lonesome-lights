#ifndef __RENDERING__PARTICLES__PARTICLE_EMITTER_H__
#define __RENDERING__PARTICLES__PARTICLE_EMITTER_H__

#include <list>
#include <glm/glm.hpp>
#include <memory>

#include "geometry/transformable.h"
#include "rendering/deferred_drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"
#include "rendering/particles/particle.h"
#include "updatable.h"

class Camera;
class RenderProgram;
class Timer;

class ParticleEmitter : public DeferredDrawable, public Updatable, public Transformable {
public:
	ParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, const std::string& fragment_shader_name, const glm::vec2& billboard_size, bool orientate_towards_velocity, bool camera_plane_aligned, unsigned int max_particle_count);

	void set_emitting(bool is_emitting);
	
	void draw(const Camera& camera) const override;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const override;
	void update(const Timer& timer) override;
protected:
	virtual void recalculate_properties() = 0;

	glm::vec3 m_particle_offset;
	glm::vec3 m_particle_start_velocity;
	glm::vec3 m_particle_acceleration;
	float m_particle_lifetime_seconds;
	float m_frequency;
private:
	bool m_is_emitting;
	bool m_orientate_towards_velocity;
	bool m_camera_plane_aligned;
	float m_current_time_seconds;
	unsigned int m_max_particle_count;
	VertexBufferObject<glm::vec2> m_base_vertex_buffer_object;
	VertexBufferObject<Particle::Data> m_instances_vertex_buffer_object;
	std::list<Particle> m_particles;
	float m_next_emission_time_seconds;
	VertexArrayObject m_vertex_array_object;
};

#endif
