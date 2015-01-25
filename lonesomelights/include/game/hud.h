#ifndef __GAME__HUD_H__
#define __GAME__HUD_H__

#include "rendering/drawable.h"

#include <glm/glm.hpp>

#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

class HUD : public Drawable {
public:
	HUD();
	
	std::pair<bool, unsigned int> get_clicked_index(float mouse_x, float mouse_y);

	void draw(const Camera& camera) const override final;
private:
	VertexBufferObject<glm::vec2> m_vbo;
	VertexArrayObject m_vao;
};

#endif
