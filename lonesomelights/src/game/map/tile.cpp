#include "game/map/tile.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "game/map/map.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"

Tile::Tile(const Map& map, unsigned int x, unsigned int y)
	: Drawable(RenderPrograms::get_render_program("tile")),
	Networkable(),
	Transformable(glm::translate(glm::vec3((float) x, 0.0F, (float) y)), map),
	m_map(map),
	m_x(x),
	m_y(y),
	m_vertices_vbo({ glm::vec3(0.0F, 0.0001F, 0.0F), glm::vec3(get_size(), 0.0001F, 0.0F), glm::vec3(get_size(), 0.0001F, get_size()), glm::vec3(0.0F, 0.0001F, get_size()) }, GL_ARRAY_BUFFER),
	m_vertex_array_object()	{
	m_vertex_array_object.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}

unsigned int Tile::get_x() const {
	return m_x;
}
unsigned int Tile::get_y() const {
	return m_y;
}
float Tile::get_size() const {
	return m_map.get_tile_size();
}
bool Tile::is_walkable() const {
	return true;
}

void Tile::draw(const Camera& camera) const {
	Drawable::draw(camera);

	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", camera);

	Drawable::m_render_program.bind();
	m_vertex_array_object.bind();
	
	glLineWidth(1.0F);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
		
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
}