#ifndef __GEOMETRY__GAME_CAMERA_H__
#define __GEOMETRY__GAME_CAMERA_H__

#include <glm/glm.hpp>

#include "geometry/perspective_camera.h"
#include "updatable.h"

class Timer;

class GameCamera : public PerspectiveCamera, public Updatable {
public:
	GameCamera(const glm::vec2& eye_position, float aspect);
	
	void set_velocity(const glm::vec2& velocity);

	void update(const Timer& timer) override final;
private:
	void change_position(const glm::vec2& offset);

	glm::vec2 m_velocity;
};

#endif