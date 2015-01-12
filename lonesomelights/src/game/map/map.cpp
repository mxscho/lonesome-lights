#include "game/map/map.h"

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/textures.h"

Map::Map(const glm::mat4& local_transformation, unsigned int width, unsigned int height)
	: Drawable(),
	Networkable(),
	Updatable(),
	Transformable(local_transformation),
	m_width(width),
	m_height(height),
	m_tiles(m_width * m_height),
	m_vertices_vbo({ glm::vec3(-1.0F, 0.0F, -1.0F), glm::vec3(1.0F, 0.0F, -1.0F), glm::vec3(1.0F, 0.0F, 1.0F), glm::vec3(-1.0F, 0.0F, 1.0F) }, GL_ARRAY_BUFFER),
	m_elements_vbo({
		0, 1, 2,
		2, 3, 0
	}, GL_ELEMENT_ARRAY_BUFFER),
	m_vertex_array_object() {
	m_vertex_array_object.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	m_elements_vbo.bind();
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}

Tile& Map::get_tile(unsigned int x, unsigned int y) {
	return *m_tiles[y * m_width + x];
}
void Map::add_tile(unsigned int x, unsigned int y, std::unique_ptr<Tile>&& tile) {
	delete_tile(x, y);
	m_tiles[y * m_width + x] = std::move(tile);
}
void Map::delete_tile(unsigned int x, unsigned int y) {
	m_tiles[y * m_width + x].reset();
}

void Map::draw(const RenderProgram& render_program, const Camera& camera) const {
	Drawable::draw(render_program, camera);
	Drawable::prepare_draw(render_program, camera);
	
	const Texture& texture = Textures::get_texture("map");
	
	render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	render_program.set_uniform("u_texture", texture.get_id());

	m_vertex_array_object.bind();
	
	texture.bind(GL_TEXTURE0);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	
	Texture::unbind_any(GL_TEXTURE0);
	
	VertexArrayObject::unbind_any();
	Drawable::finalize_draw(render_program);
}

void Map::update(const Timer& timer) {
	Updatable::update(timer);
	
	// TODO: Update map.
	
	// TEST
	Networkable::on_update();
}