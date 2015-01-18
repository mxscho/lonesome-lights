#ifndef __RENDERING__PARTICLES__PARTICLE_SYSTEMS__LASER_H__
#define __RENDERING__PARTICLES__PARTICLE_SYSTEMS__LASER_H__

#include <glm/glm.hpp>

#include "geometry/transformable.h"
#include "rendering/particles/linear_particle_emitter.h"
#include "rendering/particles/spheric_particle_emitter.h"

class Camera;
class Timer;

class Laser : public Updatable, public Transformable {
public:
	Laser(const glm::mat4& transformation, const Transformable& parent_transformable, const glm::vec3& color);
	
	void set_target(const glm::vec3& target);
	void set_enabled(bool is_enabled);
	
	void draw(const Camera& camera) const;
	
	void update(const Timer& timer) override final;
private:
	bool m_is_enabled;
	glm::vec3 m_target;
	LinearParticleEmitter m_laser;
	SphericParticleEmitter m_sparks;
};

#endif