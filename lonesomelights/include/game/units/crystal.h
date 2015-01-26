#ifndef __GAME__UNITS__CRYSTAL_H__
#define __GAME__UNITS__CRYSTAL_H__

#include "geometry/transformable.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

#include <glm/glm.hpp>

class Camera;
class Map;
class Timer;

class Crystal : public Drawable, public Transformable {
public:
	static Crystal create(const glm::mat4& transformation, const Map& map);
	
	void update(const Timer& timer);
	void draw(const Camera& camera) const override final;

private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec3& normal);
		
		glm::vec3 position;
		glm::vec3 normal;
	};
	
	Crystal(const glm::mat4& transformation, const Map& map, const std::vector<Data>& vertices);
	
	glm::vec3 m_color;
	VertexBufferObject<Data> m_crystal_vbo;
	VertexBufferObject<GLuint> m_crystal_elements_vbo;
	VertexArrayObject m_crystal_vao;
	float m_current_time_seconds;
};

#endif
