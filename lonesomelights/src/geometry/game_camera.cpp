#include "geometry/game_camera.h"

#include "timer.h"

GameCamera::GameCamera(const glm::vec2& eye_position, float aspect)
	: PerspectiveCamera(PerspectiveCamera::create_from_view_direction(glm::vec3(eye_position.x, 1.0F, eye_position.y), glm::vec3(1.0F, -1.0F, 1.0F), glm::vec3(1.0F, 1.0F, 1.0F), 70.0F, aspect, 0.0001F, 1000.0F)),
	m_velocity(0.0F) {
}

void GameCamera::set_velocity(const glm::vec2& velocity) {
	m_velocity = velocity;
}

void GameCamera::update(const Timer& timer) {
	Updatable::update(timer);
	
	float delta_time = timer.get_delta_time_seconds();
	change_position(glm::vec2(delta_time * m_velocity.x, delta_time * m_velocity.y));
}

void GameCamera::change_position(const glm::vec2& offset) {
	glm::vec3 view_direction = Camera::m_center_position - Camera::m_eye_position;
	glm::vec3 x_direction = glm::normalize(glm::vec3(-view_direction.z, 0.0F, view_direction.x));
	glm::vec3 y_direction = glm::normalize(glm::vec3(view_direction.x, 0.0F, view_direction.z));
	glm::vec3 offset2 = offset.x * x_direction + offset.y * y_direction;
	Camera::change_position(offset2);
}