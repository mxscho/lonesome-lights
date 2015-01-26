#include "game/map/base_tile.h"

#include "rendering/obj_loader/obj_loader.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

BaseTile BaseTile::create(const Map& map, unsigned int x, unsigned int y, const Player& player) {
	
	std::vector<GLfloat> base_positions = ObjLoader::get_obj_positions("base", 0);
	std::vector<GLfloat> base_normals = ObjLoader::get_obj_normals("base", 0);
	
	std::vector<BaseTile::Data> vertices;
	for (unsigned int i = 0; i < base_positions.size(); i += 3) {
		vertices.push_back(BaseTile::Data(glm::vec3(base_positions[i + 0], base_positions[i + 1], base_positions[i + 2]), glm::vec3(base_normals[i + 0], base_normals[i + 1], base_normals[i + 2])));
	}

	return BaseTile(map, x, y, vertices, player);
}

const Player& BaseTile::get_player() const {
	return m_player;
}

void BaseTile::draw(const Camera& camera) const {
	m_floor_tile.draw(camera);
	
	Drawable::draw(camera);

	bool culled = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * m_base_transformation);
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_color", m_player.get_color());

	Drawable::m_render_program.bind();
	
	m_vertex_array_object.bind();
	glDrawElements(GL_TRIANGLES, m_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	if (culled) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}
}

BaseTile::Data::Data(const glm::vec3& position, const glm::vec3& normal)
	: position(position),
	normal(normal) {
}

BaseTile::BaseTile(const Map& map, unsigned int x, unsigned int y, const std::vector<BaseTile::Data>& vertices, const Player& player)
	: Tile(map, x, y, RenderPrograms::get_render_program("unit")),
	m_vertices_vbo(vertices, GL_ARRAY_BUFFER),
	m_elements_vbo(ObjLoader::get_obj_elements("base", 0), GL_ELEMENT_ARRAY_BUFFER),
	m_vertex_array_object(),
	m_floor_tile(FloorTile(map, x, y)),
	m_base_transformation(glm::mat4(glm::translate(glm::vec3(get_size() / 2.0f,0.0f, get_size() / 2.0f))) * glm::mat4(glm::scale(glm::vec3(0.38f, 0.38f, 0.38f)))),
	m_player(player) {
	
	set_is_walkable(false);
	
	m_vertex_array_object.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BaseTile::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BaseTile::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_elements_vbo.bind();
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}
