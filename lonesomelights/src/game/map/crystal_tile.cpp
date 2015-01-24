#include "game/map/crystal_tile.h"

#include "rendering/obj_loader/obj_loader.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

CrystalTile CrystalTile::create(const Map& map, unsigned int x, unsigned int y, const RockTile::CliffType& cliff_type) {
	std::vector<GLfloat> crystals_positions = ObjLoader::get_obj_positions("crystals", 0);
	std::vector<GLfloat> crystals_normals = ObjLoader::get_obj_normals("crystals", 0);
	std::vector<GLfloat> landscape_positions = ObjLoader::get_obj_positions("crystals", 1);
	std::vector<GLfloat> landscape_normals = ObjLoader::get_obj_normals("crystals", 1);
	
	std::vector<CrystalTile::Data> vertices;
	std::vector<unsigned int> vertex_counts;
	for (unsigned int i = 0; i < crystals_positions.size(); i += 3) {
		vertices.push_back(CrystalTile::Data(glm::vec3(crystals_positions[i + 0], crystals_positions[i + 1], crystals_positions[i + 2]), glm::vec3(crystals_normals[i + 0], crystals_normals[i + 1], crystals_normals[i + 2])));
	}
	vertex_counts.push_back(crystals_positions.size() / 3);
	for (unsigned int i = 0; i < landscape_positions.size(); i += 3) {
		vertices.push_back(CrystalTile::Data(glm::vec3(landscape_positions[i + 0], landscape_positions[i + 1], landscape_positions[i + 2]), glm::vec3(landscape_normals[i + 0], landscape_normals[i + 1], landscape_normals[i + 2])));
	}
	vertex_counts.push_back(landscape_positions.size() / 3);

	return CrystalTile(map, x, y, vertices, vertex_counts, cliff_type);
}

void CrystalTile::draw(const Camera& camera) const {
	m_floor_tile.draw(camera);
	m_rock_tile.draw(camera);
	
	Drawable::draw(camera);

	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * m_crystal_tile_transformation);
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_color", glm::vec3(0.4f, 0.2f, 0.8f));

	Drawable::m_render_program.bind();
	
	m_crystals_vao.bind();
	glDrawElements(GL_TRIANGLES, m_crystals_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr);
	
	/*Drawable::m_render_program.set_uniform("u_color", glm::vec3(0.2f, 0.2f, 0.2f));
	m_landscape_vao.bind();
	glDrawElementsBaseVertex(GL_TRIANGLES, m_landscape_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr, m_vertex_counts[0]);
	*/
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	
}

CrystalTile::Data::Data(const glm::vec3& position, const glm::vec3& normal)
	: position(position),
	normal(normal) {
}

CrystalTile::CrystalTile(const Map& map, unsigned int x, unsigned int y, const std::vector<CrystalTile::Data>& vertices, const std::vector<unsigned int>& vertex_counts, const RockTile::CliffType& cliff_type)
	: Tile(map, x, y, RenderPrograms::get_render_program("unit")),
	m_vertices_vbo(vertices, GL_ARRAY_BUFFER),
	m_vertex_counts(vertex_counts),
	m_crystals_elements_vbo(ObjLoader::get_obj_elements("crystals", 0), GL_ELEMENT_ARRAY_BUFFER),
	m_landscape_elements_vbo(ObjLoader::get_obj_elements("crystals", 1), GL_ELEMENT_ARRAY_BUFFER),
	m_crystals_vao(),
	m_landscape_vao(),
	m_floor_tile(FloorTile(map, x, y)),
	m_rock_tile(RockTile::create(map, x, y, cliff_type)),
	m_crystal_tile_transformation(glm::mat4(glm::translate(glm::vec3(get_size() / 2.0f, 1.0f, get_size() / 2.0f))) * glm::mat4(glm::scale(glm::vec3(0.15f, 0.15f, 0.15f)))) {
	
	set_is_walkable(false);
	
	m_crystals_vao.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CrystalTile::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CrystalTile::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_crystals_elements_vbo.bind();
	
	m_landscape_vao.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CrystalTile::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CrystalTile::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_landscape_elements_vbo.bind();
	
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}
