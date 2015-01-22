#ifndef __RENDERING__PARTICLES__HEALTH_BAR_PARTICLE_EMITTER_H__
#define __RENDERING__PARTICLES__HEALTH_BAR_PARTICLE_EMITTER_H__

#include "rendering/particles/particle_emitter.h"

#include <glm/glm.hpp>

class Camera;
class RenderProgram;
class Texture;
class Timer;

class HealthBarParticleEmitter : public ParticleEmitter {
public:
	HealthBarParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, float max_health, float health);
	
	float get_health() const;
	void set_health(float health);
	
	void draw(const Camera& camera) const override final;
	void draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const override final;
protected:
	void recalculate_properties() override final;
private:
	float m_max_health;
	float m_health;
};

#endif