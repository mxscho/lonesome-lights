#ifndef __RENDERING__PARTICLES__SPHERIC_PARTICLE_EMITTER_H__
#define __RENDERING__PARTICLES__SPHERIC_PARTICLE_EMITTER_H__

#include "rendering/particles/particle_emitter.h"

class Camera;
class RenderProgram;
class Timer;

class SphericParticleEmitter : public ParticleEmitter {
public:
	SphericParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, float billboard_size, float current_time_seconds,
		float radius, float particle_start_velocity, float particle_acceleration, float min_particle_lifetime_seconds, float max_particle_lifetime_seconds, float frequency);
protected:
	void recalculate_properties() override final;
private:
	float m_radius;
	float m_particle_start_velocity;
	float m_particle_acceleration;
	float m_min_particle_lifetime_seconds;
	float m_max_particle_lifetime_seconds;
};

#endif