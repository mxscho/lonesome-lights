#ifndef __GAME__UNITS__UNIT_H__
#define __GAME__UNITS__UNIT_H__

#include "game/units/selection_circle.h"
#include "geometry/inertial_movable.h"
#include "networking/networkable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

#include <list>
#include <glm/glm.hpp>

class Camera;
class Map;
class Player;
class RenderProgram;
class Timer;

class Unit : public Networkable, public InertialMovable {
public:
	Unit(const glm::mat4& transformation, const glm::vec2& position, const Map& map, const Player& player, float max_velocity, float acceleration, float decceleration);

	glm::vec2 get_position_vec2() const;
	bool is_selected() const;
	void set_selected(bool is_selected);
	
	glm::vec2 get_target_position_vec2() const;
	void set_target_path(const Timer& timer, const std::list<glm::vec2>& target_path);
	void set_target_position(const Timer& timer, const glm::vec2& target_position);
	void add_target_position_to_path(const Timer& timer, const glm::vec2& target_position);
	void add_target_path_to_path(const Timer& timer, const std::list<glm::vec2>& target_path);
	
	void draw(const Camera& camera) const;
	
	void update(const Timer& timer) override;
protected:
	const Player& m_player;
	bool m_is_selected;
	SelectionCircle m_selection_circle;
};

#endif