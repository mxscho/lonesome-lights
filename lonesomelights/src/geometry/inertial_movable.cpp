#include "geometry/inertial_movable.h"

#include "timer.h"

InertialMovable::InertialMovable(const glm::mat4& transformation, const Transformable& parent_transformable, float max_velocity, float acceleration, float decceleration)
	: Updatable(),
	Transformable(transformation, parent_transformable),
	m_max_velocity(max_velocity),
	m_acceleration(acceleration),
	m_decceleration(decceleration),
	m_velocity(0.0F),
	m_start_position(),
	m_start_velocity(),
	m_start_time_seconds(),
	m_target_position(Transformable::get_position()),
	m_movement(nullptr) {
}
void InertialMovable::set_target_position(const Timer& timer, const glm::vec3& target_position) {
	m_start_position = Transformable::get_position();
	if (m_movement) {
		m_start_velocity = m_velocity;
	} else {
		m_start_velocity = 0.0F;
	}
	m_start_time_seconds = timer.get_current_time_seconds();
	m_target_position = target_position;
	
	m_movement.reset(new InertialMovement(0.0F, m_start_velocity, 1.0F, 0.0F, m_max_velocity, m_acceleration, m_decceleration));
}
void InertialMovable::update(const Timer& timer) {
	if (!m_movement) {
		return;
	}

	Movement::State movement_state = m_movement->calculate(Movement::State(0.0F, m_start_velocity), 0.0F, timer.get_current_time_seconds() - m_start_time_seconds);
	Transformable::set_position(m_start_position + movement_state.position * (m_target_position - m_start_position));
	m_velocity = movement_state.velocity;
	if (movement_state.position >= 1.0F) {
		Transformable::set_position(m_target_position);
		m_velocity = 0.0F;
		m_movement.reset();
	}
}