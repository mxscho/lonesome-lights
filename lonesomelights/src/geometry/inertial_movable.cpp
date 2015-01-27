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
	m_start_time_seconds(),
	m_target_path(),
	m_constant_movement_time_seconds(),
	m_acceleration_time_seconds(),
	m_decceleration_time_seconds(),
	m_already_moved_distance(),
	m_is_moving(false) {
}

bool InertialMovable::is_moving() const {
	return m_is_moving;
}

glm::vec3 InertialMovable::get_target_position() const {
	return m_target_path.size() > 0 ? m_target_path.back() : Transformable::get_position();
}
std::list<glm::vec3>& InertialMovable::get_target_path() {
	return m_target_path;
}
void InertialMovable::set_target_path(const Timer& timer, const std::list<glm::vec3>& target_path) {
	m_start_position = Transformable::get_position();
	m_start_velocity = m_velocity;
	m_start_time_seconds = timer.get_current_time_seconds();
	m_target_path = target_path;

	float position_to_last_target_position_distance = 0.0F;
	glm::vec3 current_target_position = m_start_position;
	for (auto i_target_position = m_target_path.begin(); i_target_position != m_target_path.end(); ) {
		float distance = glm::distance(current_target_position, *i_target_position);
		if (distance < 0.00001F) {
			i_target_position = m_target_path.erase(i_target_position);
		} else {
			position_to_last_target_position_distance += distance;
			current_target_position = *i_target_position;
			++i_target_position;
		}
	}
	
	// Calculate maximum possible velocity considering the distance to the target and the acceleration/decceleration.
	float max_velocity = std::sqrt(m_decceleration * (2.0F * m_acceleration * position_to_last_target_position_distance + m_start_velocity * m_start_velocity) / (m_acceleration + m_decceleration));
	max_velocity = std::min(max_velocity, m_max_velocity);
	
	m_acceleration_time_seconds = (max_velocity - m_start_velocity) / m_acceleration;
	
	float acceleration_distance = 0.5F * ((max_velocity * max_velocity) - (m_start_velocity * m_start_velocity)) / m_acceleration;
	float decceleration_distance = 0.5F * (max_velocity * max_velocity) / m_decceleration;
	float acceleration_decceleration_distance = acceleration_distance + decceleration_distance;
	float constant_movement_distance = position_to_last_target_position_distance - acceleration_decceleration_distance;
	
	m_constant_movement_time_seconds = constant_movement_distance / max_velocity;
	m_decceleration_time_seconds = max_velocity / m_decceleration;
	
	if (m_start_velocity > max_velocity) {
		// It's not possible to deccelerate in time.
		m_acceleration_time_seconds = 0.0F;
		m_constant_movement_time_seconds = 0.0F;
		m_decceleration_time_seconds = m_start_velocity / m_decceleration;
	}
	
	m_already_moved_distance = 0.0F;
	m_is_moving = true;
}
void InertialMovable::add_target_position_to_path(const Timer& timer, const glm::vec3& target_position) {
	std::list<glm::vec3> target_path = m_target_path;
	target_path.push_back(target_position);
	set_target_path(timer, target_path);
}
void InertialMovable::add_target_path_to_path(const Timer& timer, const std::list<glm::vec3>& target_path) {
	std::list<glm::vec3> new_target_path = m_target_path;
	new_target_path.insert(new_target_path.end(), target_path.begin(), target_path.end());
	set_target_path(timer, new_target_path);
}
void InertialMovable::set_target_position(const Timer& timer, const glm::vec3& target_position) {
	set_target_path(timer, { target_position });
}

void InertialMovable::update(const Timer& timer) {
	if (!m_is_moving) {
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
	
	float decceleration_time_seconds = std::min(m_decceleration_time_seconds, time_since_start_seconds);
	state = m_deccelerating_movement.calculate(state, 0.0F, decceleration_time_seconds);
	time_since_start_seconds -= decceleration_time_seconds;

	while (m_target_path.size() > 0) {
		m_velocity = state.velocity;

		glm::vec3 position_to_target_position = m_target_path.front() - m_start_position;
		float position_to_target_position_length = glm::length(position_to_target_position);
		if (m_target_path.size() == 1 || state.displacement - m_already_moved_distance < position_to_target_position_length) {
			Transformable::set_position(m_start_position + (state.displacement - m_already_moved_distance) * glm::normalize(position_to_target_position));
			break;
		} else {
			Transformable::set_position(m_target_path.front());
			m_start_position = m_target_path.front();
			m_target_path.pop_front();
			m_already_moved_distance += position_to_target_position_length;
		}
	}
	
	if (m_velocity <= 0.0F) {
		m_velocity = 0.0F;
		m_is_moving = false;
	}
}
