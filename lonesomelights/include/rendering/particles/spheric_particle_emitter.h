#ifndef __RENDERING__PARTICLES__SPHERIC_PARTICLE_EMITTER_H__
#define __RENDERING__PARTICLES__SPHERIC_PARTICLE_EMITTER_H__

#include "rendering/particles/particle_emitter.h"

#include <glm/glm.hpp>

class Camera;
class RenderProgram;
class Texture;
class Timer;

class SphericParticleEmitter : public ParticleEmitter {
public:
	SphericParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, const glm::vec2& billboard_size,
		const Texture& texture, const glm::vec3& start_color, const glm::vec3& end_color, float radius, float particle_start_velocity, float gravity, float min_particle_lifetime_seconds, float max_particle_lifetime_seconds, float frequency);
	
	void draw(const Camera& camera) const override final;
protected:
	void recalculate_properties() override final;
private:
	const Texture& m_texture;
	glm::vec3 m_start_color;
	glm::vec3 m_end_color;
	float m_radius;
	float m_particle_start_velocity;
	float m_gravity;
	float m_min_particle_lifetime_seconds;
	float m_max_particle_lifetime_seconds;
};

#endif