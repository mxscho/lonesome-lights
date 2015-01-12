#include "geometry/camera.h"

#include "rendering/opengl/render_program.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

void Camera::bind_to_render_program(const RenderProgram& render_program, const std::string& uniform_view_transformation_name, const std::string& uniform_projection_transformation_name) const {
	render_program.set_uniform(uniform_view_transformation_name, m_view_transformation);
	render_program.set_uniform(uniform_projection_transformation_name, m_projection_transformation);
}

Camera Camera::create_from_view_direction(const glm::vec3& eye_position, const glm::vec3& view_direction, const glm::vec3& up_direction, const glm::mat4& projection_transformation) {
	return Camera(eye_position, eye_position + view_direction, up_direction, projection_transformation);
}

Camera::Camera(const glm::vec3& eye_position, const glm::vec3& center_position, const glm::vec3& up_direction, const glm::mat4& projection_transformation)
	: m_eye_position(eye_position),
	m_center_position(center_position),
	m_up_direction(up_direction),
	m_view_transformation(glm::lookAt(m_eye_position, m_center_position, m_up_direction)),
	m_projection_transformation(projection_transformation) {
}

void Camera::change_position(const glm::vec3& offset) {
	m_eye_position += offset;
	m_center_position += offset;
	m_view_transformation = glm::lookAt(m_eye_position, m_center_position, m_up_direction);
}