#ifndef __GAME__UNITS__UNIT_H__
#define __GAME__UNITS__UNIT_H__

#include "geometry/inertial_movable.h"
#include "networking/networkable.h"
#include "rendering/drawable.h"

#include <list>
#include <glm/glm.hpp>

class Camera;
class Map;
class RenderProgram;
class Timer;

class Unit : public Drawable, public Networkable, public InertialMovable {
public:
	Unit(const glm::mat4& transformation, const glm::vec2& position, const Map& map, float max_velocity, float acceleration, float decceleration);

	glm::vec2 get_position_vec2() const;
	
	void set_target_path(const Timer& timer, const std::list<glm::vec2>& target_path);
	void set_target_position(const Timer& timer, const glm::vec2& target_position);
	void add_target_position_to_path(const Timer& timer, const glm::vec2& target_position);
	
	void update(const Timer& timer) override;
};

#endif