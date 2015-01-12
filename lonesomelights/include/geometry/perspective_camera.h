#ifndef __GEOMETRY__PERSPECTIVE_CAMERA_H__
#define __GEOMETRY__PERSPECTIVE_CAMERA_H__

#include <glm/glm.hpp>

#include "geometry/camera.h"

class PerspectiveCamera : public Camera {
public:
	static PerspectiveCamera create_from_view_direction(const glm::vec3& eye_position, const glm::vec3& view_direction, const glm::vec3& up_direction, float fov, float aspect, float near, float far);

	PerspectiveCamera(const glm::vec3& eye_position, const glm::vec3& center_position, const glm::vec3& up_direction, float fov, float aspect, float near, float far);
};

#endif