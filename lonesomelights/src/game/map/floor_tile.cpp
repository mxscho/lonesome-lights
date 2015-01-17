#include "game/map/floor_tile.h"

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"

FloorTile::FloorTile(const Map& map, unsigned int x, unsigned int y)
	: Tile(map, x, y, RenderPrograms::get_render_program("floor_tile")),
	m_vertices_vbo({ glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 0.0F, get_size()), glm::vec3(get_size(), 0.0F, get_size()), glm::vec3(get_size(), 0.0F, 0.0F) }, GL_ARRAY_BUFFER),
	m_vertex_array_object()	{
	m_vertex_array_object.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}

bool FloorTile::is_walkable() const {
	return true;
}

void FloorTile::draw(const Camera& camera) const {
	Drawable::draw(camera);

	const Texture& texture = Textures::get_texture("floor_tile");
	
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniform("u_texture", texture.get_id());
	
	Drawable::m_render_program.bind();
	m_vertex_array_object.bind();
	
	texture.bind(GL_TEXTURE0);
	
	glDrawArrays(GL_QUADS, 0, 4);
	
	Texture::unbind_any(GL_TEXTURE0);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
}