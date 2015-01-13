#include "geometry/inertial_movement.h"

#include <cmath>

InertialMovement::InertialMovement(float start_position, float start_velocity, float end_position, float end_velocity, float max_velocity, float acceleration, float decceleration)
	: Movement(),
	m_start_position(start_position),
	m_start_velocity(start_velocity),
	m_end_position(end_position),
	m_end_velocity(end_velocity),
	m_max_velocity(max_velocity),
	m_acceleration(acceleration),
	m_decceleration(decceleration) {
}
float InertialMovement::calculate_acceleration(float position, float velocity, float time) {
	float acceleration_distance = (m_max_velocity - m_start_velocity) / m_acceleration;
	float decceleration_distance = (m_max_velocity - m_end_velocity) / m_decceleration;
	float acceleration_decceleration_distance = acceleration_distance + decceleration_distance;
	float position_distance = m_end_position - m_start_position;
	if (acceleration_decceleration_distance > position_distance) {
		acceleration_distance = (acceleration_distance / acceleration_decceleration_distance) * position_distance;
		decceleration_distance = (decceleration_distance / acceleration_decceleration_distance) * position_distance;
	}
	if (position <= acceleration_distance) {
		return m_acceleration;
	} else if (position >= m_end_position - decceleration_distance) {
		return -m_decceleration;
	} else {
		// We need to correct the inaccuracy if existing.
		return std::abs(m_max_velocity - velocity);
	}
}