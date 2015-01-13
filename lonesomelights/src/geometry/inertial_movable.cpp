#include "geometry/inertial_movable.h"

#include "timer.h"

#include <cmath>

InertialMovable::InertialMovable(const glm::mat4& transformation, const Transformable& parent_transformable, float max_velocity, float acceleration, float decceleration)
	: Updatable(),
	Transformable(transformation, parent_transformable),
	m_velocity(0.0F),
	m_max_velocity(max_velocity),
	m_acceleration(acceleration),
	m_decceleration(decceleration),
	m_constant_movement(),
	m_accelerating_movement(m_acceleration),
	m_deccelerating_movement(-m_decceleration),
	m_start_position(),
	m_start_velocity(),
	m_start_time_seconds(0.0F),
	m_target_position(Transformable::get_position()),
	m_constant_movement_time_seconds(0.0F),
	m_acceleration_time_seconds(0.0F) {
}

void InertialMovable::set_target_position(const Timer& timer, const glm::vec3& target_position) {
	m_start_position = Transformable::get_position();
	m_start_velocity = m_velocity;
	m_start_time_seconds = timer.get_current_time_seconds();
	m_target_position = target_position;

	float position_to_target_position_distance = glm::distance(m_start_position, m_target_position);
	
	// Calculate maximum possible velocity considering the distance to the target and the acceleration/decceleration.
	float max_velocity = std::sqrt(m_decceleration * (2.0F * m_acceleration * position_to_target_position_distance + m_start_velocity * m_start_velocity) / (m_acceleration + m_decceleration));
	max_velocity = std::min(max_velocity, m_max_velocity);
	
	m_acceleration_time_seconds = (max_velocity - m_start_velocity) / m_acceleration;
	
	float acceleration_distance = 0.5F * ((max_velocity * max_velocity) - (m_start_velocity * m_start_velocity)) / m_acceleration;
	float decceleration_distance = 0.5F * (max_velocity * max_velocity) / m_decceleration;
	float acceleration_decceleration_distance = acceleration_distance + decceleration_distance;
	float constant_movement_distance = position_to_target_position_distance - acceleration_decceleration_distance;
	
	m_constant_movement_time_seconds = constant_movement_distance / max_velocity;
}

void InertialMovable::update(const Timer& timer) {
	if (Transformable::get_position() == m_target_position) {
		return;
	}
	
	float time_since_start_seconds = timer.get_current_time_seconds() - m_start_time_seconds;
	Movement::State state(0.0F, m_start_velocity);
	
	float acceleration_time_seconds = std::min(m_acceleration_time_seconds, time_since_start_seconds);
	state = m_accelerating_movement.calculate(state, 0.0F, acceleration_time_seconds);
	time_since_start_seconds -= acceleration_time_seconds;
	
	float constant_movement_time_seconds = std::min(m_constant_movement_time_seconds, time_since_start_seconds);
	state = m_constant_movement.calculate(state, 0.0F, constant_movement_time_seconds);
	time_since_start_seconds -= constant_movement_time_seconds;
	
	state = m_deccelerating_movement.calculate(state, 0.0F, time_since_start_seconds);
	
	glm::vec3 position_to_target_position = m_target_position - m_start_position;
	Transformable::set_position(m_start_position + state.displacement * glm::normalize(position_to_target_position));
	m_velocity = state.velocity;
	
	if (m_velocity <= 0.0F) {
		Transformable::set_position(m_target_position);
		m_velocity = 0.0F;
	}
}