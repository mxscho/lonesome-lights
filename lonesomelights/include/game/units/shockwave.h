#ifndef __GAME__UNITS__SHOCKWAVE_H__
#define __GAME__UNITS__SHOCKWAVE_H__

#include "geometry/transformable.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"
#include "updatable.h"

#include <glm/glm.hpp>

class Camera;
class Map;
class Timer;

class Shockwave : public Drawable, public Updatable, public Transformable {
public:
	Shockwave(const glm::mat4& transformation, const Map& map, const glm::vec3& color);
	
	void update(const Timer& timer) override final;
	
	void draw(const Camera& camera) const override final;
private:
	static unsigned int m_instance_count;
	unsigned int m_instance_index;

	glm::vec3 m_color;
	VertexBufferObject<glm::vec2> m_shockwave_vbo;
	VertexArrayObject m_shockwave_vao;
	float m_current_time_seconds;
};

#endif