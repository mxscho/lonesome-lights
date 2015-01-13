#include "geometry/perspective_camera.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

PerspectiveCamera PerspectiveCamera::create_from_view_direction(const glm::vec3& eye_position, const glm::vec3& view_direction, const glm::vec3& up_direction, float fov, float aspect, float near, float far) {
	return PerspectiveCamera(eye_position, eye_position + view_direction, up_direction, fov, aspect, near, far);
}

PerspectiveCamera::PerspectiveCamera(const glm::vec3& eye_position, const glm::vec3& center_position, const glm::vec3& up_direction, float fov, float aspect, float near, float far)
	: Camera(eye_position, center_position, up_direction, glm::perspective(fov, aspect, near, far)) {
}