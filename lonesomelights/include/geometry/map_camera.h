#ifndef __GEOMETRY__MAP_CAMERA_H__
#define __GEOMETRY__MAP_CAMERA_H__

#include <glm/glm.hpp>

#include "geometry/perspective_camera.h"
#include "geometry/transformable.h"
#include "updatable.h"

class Map;
class Timer;

class MapCamera : public PerspectiveCamera, public Updatable, public Transformable {
public:
	MapCamera(const Map& map, const glm::vec2& eye_position, float aspect);
	
	void set_velocity(const glm::vec2& velocity);
	void change_zoom(float zoom_delta);

	void update(const Timer& timer) override final;
private:
	void change_position(const glm::vec3& offset);

	glm::vec2 m_velocity;
};

#endif