#include "game/map/map.h"

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"

#include <glm/glm.hpp>

Map::Map(unsigned int tile_count_x, unsigned int tile_count_y, float tile_size)
	: Drawable(RenderPrograms::get_render_program("map")),
	Networkable(),
	Updatable(),
	Transformable(glm::mat4()),
	m_tile_count_x(tile_count_x),
	m_tile_count_y(tile_count_y),
	m_tile_size(tile_size),
	m_tiles(m_tile_count_x * m_tile_count_y),
	m_vertices_vbo({ glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(m_tile_count_x, 0.0F, 0.0F), glm::vec3(m_tile_count_x, 0.0F, m_tile_count_y), glm::vec3(0.0F, 0.0F, m_tile_count_y) }, GL_ARRAY_BUFFER),
	m_elements_vbo({
		0, 1, 2,
		2, 3, 0
	}, GL_ELEMENT_ARRAY_BUFFER),
	m_vertex_array_object() {
	for (unsigned int i_y = 0; i_y < m_tile_count_y; ++i_y) {
		for (unsigned int i_x = 0; i_x < m_tile_count_x; ++i_x) {
			set_tile(std::unique_ptr<Tile>(new Tile(*this, i_x, i_y)));
		}
	}
	
	m_vertex_array_object.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	m_elements_vbo.bind();
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	const Texture& texture = Textures::get_texture("map");
	Drawable::m_render_program.set_uniform("u_texture", texture.get_id());
}

unsigned int Map::get_tile_count_x() const {
	return m_tile_count_x;
}
unsigned int Map::get_tile_count_y() const {
	return m_tile_count_y;
}
float Map::get_tile_size() const {
	return m_tile_size;
}
Tile& Map::get_tile(unsigned int x, unsigned int y) {
	assert(!!m_tiles[y * m_tile_count_x + x]);
	return *m_tiles[y * m_tile_count_x + x];
}
void Map::set_tile(std::unique_ptr<Tile>&& tile) {
	unsigned int x = tile->get_x();
	unsigned int y = tile->get_y();
	delete_tile(x, y);
	m_tiles[y * m_tile_count_x + x] = std::move(tile);
}
void Map::delete_tile(unsigned int x, unsigned int y) {
	m_tiles[y * m_tile_count_x + x].reset();
}

void Map::draw(const Camera& camera) const {
	Drawable::draw(camera);

	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", camera);

	Drawable::m_render_program.bind();
	m_vertex_array_object.bind();
	
	const Texture& texture = Textures::get_texture("map");
	texture.bind(GL_TEXTURE0);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	
	Texture::unbind_any(GL_TEXTURE0);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	
	for (auto& i_tile : m_tiles) {
		if (!!i_tile) {
			i_tile->draw(camera);
		}
	}
}

void Map::update(const Timer& timer) {
	Updatable::update(timer);
	
	// TODO: Update map.
	
	// TEST
	Networkable::on_update();
}