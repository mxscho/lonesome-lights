#ifndef __RENDERING__PARTICLES__PARTICLE_SYSTEMS__EXPLOSION_H__
#define __RENDERING__PARTICLES__PARTICLE_SYSTEMS__EXPLOSION_H__

#include <glm/glm.hpp>

#include "geometry/transformable.h"
#include "rendering/particles/spheric_particle_emitter.h"

class Camera;
class Timer;

class Explosion : public Updatable, public Transformable {
public:
	static const float c_end_time_seconds;
	static const float c_smoke_trails_start_time_seconds;
	static const float c_smoke_trails_end_time_seconds;
	static const float c_sparks_start_time_seconds;
	static const float c_sparks_end_time_seconds;
	static const float c_flames_start_time_seconds;
	static const float c_flames_end_time_seconds;
	static const float c_flash_start_time_seconds;
	static const float c_flash_end_time_seconds;
	static const float c_smoke_start_time_seconds;
	static const float c_smoke_end_time_seconds;

	Explosion(const glm::mat4& transformation, const Transformable& parent_transformable, float intensity);
	
	void trigger(float current_time_seconds);
	bool has_finished() const;
	
	void draw(const Camera& camera) const;
	
	void update(const Timer& timer) override final;
private:
	bool m_has_finished;
	float m_start_time_seconds;
	float m_max_particle_lifetime_seconds;
	SphericParticleEmitter m_smoke_trails;
	SphericParticleEmitter m_sparks;
	SphericParticleEmitter m_flames;
	SphericParticleEmitter m_flash;
	SphericParticleEmitter m_smoke;
};

#endif