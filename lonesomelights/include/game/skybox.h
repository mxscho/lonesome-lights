#ifndef __GAME__SKYBOX_H__
#define __GAME__SKYBOX_H__

#include "geometry/transformable.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

#include <glm/glm.hpp>

class Camera;
class Map;

class Skybox : public Drawable, public Transformable {
public:
	Skybox(const glm::mat4& transformation, const Map& map);
	
	void draw(const Camera& camera) const override final;

private:
	struct Data {
		Data(const glm::vec3& position, const glm::vec2& texel);
		
		glm::vec3 position;
		glm::vec2 texel;
	};
	
	VertexBufferObject<Data> m_up_vbo;
	VertexArrayObject m_up_vao;
	VertexBufferObject<Data> m_bottom_vbo;
	VertexArrayObject m_bottom_vao;
	VertexBufferObject<Data> m_left_vbo;
	VertexArrayObject m_left_vao;
	VertexBufferObject<Data> m_right_vbo;
	VertexArrayObject m_right_vao;
	VertexBufferObject<Data> m_front_vbo;
	VertexArrayObject m_front_vao;
	VertexBufferObject<Data> m_back_vbo;
	VertexArrayObject m_back_vao;
};

#endif
