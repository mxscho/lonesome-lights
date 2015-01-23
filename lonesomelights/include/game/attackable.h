#ifndef __GAME__ATTACKABLE_H__
#define __GAME__ATTACKABLE_H__

#include <glm/glm.hpp>

#include "rendering/particles/health_bar_particle_emitter.h"

class Camera;
class Timer;
class Transformable;

class Attackable {
public:
	Attackable(const glm::mat4& transformation, const Transformable& parent_transformable, float max_health, float health);

	virtual glm::vec2 get_position_vec2() const = 0;
	
	void change_health(float delta_health);
	bool is_dead() const;
	
	void draw(const Camera& camera) const;
	
	void update(const Timer& timer);
private:
	HealthBarParticleEmitter m_health_bar;
};

#endif
