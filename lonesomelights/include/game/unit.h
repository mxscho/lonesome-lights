#ifndef __GAME__UNIT_H__
#define __GAME__UNIT_H__

#include "geometry/inertial_movable.h"
#include "networking/networkable.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

#include <glm/glm.hpp>

class Camera;
class Map;
class RenderProgram;
class Timer;

class Unit : public Drawable, public Networkable, public InertialMovable {
public:
	Unit(const glm::vec2& position, const Map& map, float max_velocity, float acceleration, float decceleration);

	void draw(const Camera& camera) const override final;
	
	void update(const Timer& timer) override final;
private:
	VertexBufferObject<glm::vec3> m_vertices_vbo;
	VertexArrayObject m_vertex_array_object;
};

#endif