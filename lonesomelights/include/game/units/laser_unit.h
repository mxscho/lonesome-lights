#ifndef __GAME__UNITS__LASER_UNIT_H__
#define __GAME__UNITS__LASER_UNIT_H__

#include "game/units/unit.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

#include <list>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class LaserUnit : public Unit {
public:
	static std::unique_ptr<LaserUnit> create(const glm::vec2& position, const Map& map);

	void draw(const Camera& camera) const override final;
	
	void update(const Timer& timer) override final;
private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec3& normal);
		
		glm::vec3 position;
		glm::vec3 normal;
	};
	
	LaserUnit(const glm::vec2& position, const Map& map, const std::vector<Data>& vertices, const std::vector<GLuint>& elements);

	VertexBufferObject<Data> m_vertices_vbo;
	VertexBufferObject<GLuint> m_elements_vbo;
	VertexArrayObject m_vertex_array_object;
};

#endif