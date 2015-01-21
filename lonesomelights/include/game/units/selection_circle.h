#ifndef __GAME__UNITS__SELECTION_CIRCLE_H__
#define __GAME__UNITS__SELECTION_CIRCLE_H__

#include "geometry/transformable.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

#include <glm/glm.hpp>

class Camera;
class Map;

class SelectionCircle : public Drawable, public Transformable {
public:
	SelectionCircle(const glm::mat4& transformation, const Map& map);

	void draw(const Camera& camera) const override final;
protected:
	VertexBufferObject<glm::vec2> m_selected_circle_vbo;
	VertexArrayObject m_selected_circle_vao;
};

#endif