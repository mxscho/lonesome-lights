#ifndef __GEOMETRY__INERTIAL_MOVABLE_H__
#define __GEOMETRY__INERTIAL_MOVABLE_H__

#include <list>
#include <glm/glm.hpp>

#include "geometry/constant_accelerating_movement.h"
#include "geometry/constant_movement.h"
#include "geometry/transformable.h"
#include "updatable.h"

class Timer;

class InertialMovable : public Updatable, public Transformable {
public:
	static float c_epsilon;

	InertialMovable(const glm::mat4& transformation, const Transformable& parent_transformable, float max_velocity, float acceleration, float decceleration);
	
	bool is_moving() const;
	
	void set_target_path(const Timer& timer, const std::list<glm::vec3>& target_path);
	void add_target_position_to_path(const Timer& timer, const glm::vec3& target_position);
	void set_target_position(const Timer& timer, const glm::vec3& target_path);
	
	virtual void update(const Timer& timer);
private:
	float m_velocity;
	float m_max_velocity;
	float m_acceleration;
	float m_decceleration;
	ConstantMovement m_constant_movement;
	ConstantAcceleratingMovement m_accelerating_movement;
	ConstantAcceleratingMovement m_deccelerating_movement;
	glm::vec3 m_start_position;
	float m_start_velocity;
	float m_start_time_seconds;
	std::list<glm::vec3> m_target_path;
	bool m_is_target_set;
	float m_constant_movement_time_seconds;
	float m_acceleration_time_seconds;
	float m_decceleration_time_seconds;
	float m_already_moved_distance;
	bool m_is_moving;
};

#endif