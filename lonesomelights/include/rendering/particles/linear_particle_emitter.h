#ifndef __RENDERING__PARTICLES__LINEAR_PARTICLE_EMITTER_H__
#define __RENDERING__PARTICLES__LINEAR_PARTICLE_EMITTER_H__

#include "rendering/particles/particle_emitter.h"

#include <glm/glm.hpp>

class Camera;
class RenderProgram;
class Texture;
class Timer;

class LinearParticleEmitter : public ParticleEmitter {
public:
	LinearParticleEmitter(const glm::mat4& transformation, const Transformable& parent_transformable, const glm::vec2& billboard_size,
	const Texture& texture, const glm::vec3& start_color, const glm::vec3& end_color, float particle_velocity, unsigned int max_particle_count, float frequency);
	
	void set_target(const glm::vec3& target);
	
	void draw(const Camera& camera) const override final;
protected:
	void recalculate_properties() override final;
private:
	const Texture& m_texture;
	glm::vec3 m_start_color;
	glm::vec3 m_end_color;
	float m_particle_velocity;
	glm::vec3 m_target;
};

#endif