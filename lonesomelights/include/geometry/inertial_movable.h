#include <glm/glm.hpp>
#include <memory>

#include "geometry/inertial_movement.h"
#include "updatable.h"
#include "geometry/transformable.h"

class Timer;

class InertialMovable : public Updatable, public Transformable {
public:
	InertialMovable(const glm::mat4& transformation, const Transformable& parent_transformable, float max_velocity, float acceleration, float decceleration);
	
	void set_target_position(const Timer& timer, const glm::vec3& target_position);
	
	void update(const Timer& timer) override;
private:
	float m_max_velocity;
	float m_acceleration;
	float m_decceleration;
	float m_velocity;
	glm::vec3 m_start_position;
	float m_start_velocity;
	float m_start_time_seconds;
	glm::vec3 m_target_position;
	std::unique_ptr<InertialMovement> m_movement;
};