#ifndef __RENDERING__PARTICLES__PARTICLE_SYSTEMS__SMOKE_H__
#define __RENDERING__PARTICLES__PARTICLE_SYSTEMS__SMOKE_H__

#include <glm/glm.hpp>

#include "geometry/transformable.h"
#include "rendering/particles/spheric_particle_emitter.h"

class Camera;
class Timer;

class Smoke : public Updatable, public Transformable {
public:
	static const float c_end_time_seconds;
	static const float c_smoke_start_time_seconds;
	static const float c_smoke_end_time_seconds;

	Smoke(const glm::mat4& transformation, const Transformable& parent_transformable, float intensity);
	
	void trigger(float current_time_seconds);
	bool has_finished() const;
	
	void draw(const Camera& camera) const;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const;
	
	void update(const Timer& timer) override final;
private:
	bool m_has_finished;
	float m_start_time_seconds;
	float m_max_particle_lifetime_seconds;
	SphericParticleEmitter m_smoke;
};

#endif
