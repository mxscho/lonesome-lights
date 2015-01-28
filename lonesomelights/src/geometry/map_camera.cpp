#include "geometry/map_camera.h"

#include "game/map/map.h"
#include "timer.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

MapCamera::MapCamera(const Map& map, const glm::vec2& eye_position, float aspect)
	: PerspectiveCamera(PerspectiveCamera::create_from_view_direction(glm::vec3(eye_position.x, 7.5F, eye_position.y), glm::normalize(glm::vec3(1.0F, -2.0F, 1.0F)), glm::normalize(glm::vec3(2.0F, 1.0F, 2.0F)), 70.0F, aspect, 0.1F, 100.0F)),
	Updatable(),
	m_velocity(0.0F) {
}

void MapCamera::set_velocity(const glm::vec2& velocity) {
	m_velocity = velocity;
}
void MapCamera::change_zoom(float zoom_delta) {
	change_position(glm::vec3(0.0F, 0.0F, zoom_delta));
}

void MapCamera::update(const Timer& timer) {
	Updatable::update(timer);
	
	float delta_time = timer.get_delta_time_seconds();
	change_position(glm::vec3(delta_time * (m_eye_position.y / 10.0F) * m_velocity.x, delta_time * (m_eye_position.y / 10.0F) * m_velocity.y, 0.0F));
}

void MapCamera::change_position(const glm::vec3& offset) {
	glm::vec3 view_direction = Camera::m_center_position - Camera::m_eye_position;
	glm::vec3 x_direction = glm::normalize(glm::vec3(-view_direction.z, 0.0F, view_direction.x));
	glm::vec3 y_direction = glm::normalize(glm::vec3(view_direction.x, 0.0F, view_direction.z));
	glm::vec3 offset2 = offset.x * x_direction + offset.y * y_direction + offset.z * view_direction;
	
	m_eye_position += offset2;
	m_center_position += offset2;
	if (m_eye_position.x > 27.0F) {
		m_eye_position.x -= offset2.x;
		m_center_position.x -= offset2.x;
	}
	if (m_eye_position.x < -3.5F) {
		m_eye_position.x -= offset2.x;
		m_center_position.x -= offset2.x;
	}
	if (m_eye_position.y > 20.0F) { // TODO: niedriger, zB 10 - 15
		m_eye_position -= offset2;
		m_center_position -= offset2;
	}
	if (m_eye_position.y < 0.5F) {
		m_eye_position -= offset2;
		m_center_position -= offset2;
	}
	if (m_eye_position.z > 27.0F) {
		m_eye_position.z -= offset2.z;
		m_center_position.z -= offset2.z;
	}
	if (m_eye_position.z < -4.0F) {
		m_eye_position.z -= offset2.z;
		m_center_position.z -= offset2.z;
	}
	m_view_transformation = glm::lookAt(m_eye_position, m_center_position, m_up_direction);
}
