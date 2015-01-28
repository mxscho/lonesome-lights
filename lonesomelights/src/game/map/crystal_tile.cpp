#include "game/map/crystal_tile.h"

#include "rendering/obj_loader/obj_loader.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <iostream>

CrystalTile CrystalTile::create(const Map& map, unsigned int x, unsigned int y, const RockTile::CliffType& cliff_type) {
	std::vector<GLfloat> crystals_positions = ObjLoader::get_obj_positions("crystals", 0);
	std::vector<GLfloat> crystals_normals = ObjLoader::get_obj_normals("crystals", 0);
	
	std::vector<CrystalTile::Data> vertices;
	for (unsigned int i = 0; i < crystals_positions.size(); i += 3) {
		vertices.push_back(CrystalTile::Data(glm::vec3(crystals_positions[i + 0], crystals_positions[i + 1], crystals_positions[i + 2]), glm::vec3(crystals_normals[i + 0], crystals_normals[i + 1], crystals_normals[i + 2])));
	}


	return CrystalTile(map, x, y, vertices, cliff_type);
}

glm::vec2 CrystalTile::get_position_vec2() const {
	return glm::vec2(get_x() + 0.5F * get_size(), get_y() + 0.5F * get_size());
}

void CrystalTile::update(const Timer& timer) {
	Attackable::update(timer);
}

void CrystalTile::draw(const Camera& camera) const {
	m_destructible_rock_tile.draw(camera);
	
	Drawable::draw(camera);
}

void CrystalTile::draw_crystals(const Camera& camera) const {
	bool culled = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	bool depth_tested = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * m_crystal_tile_transformation);
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_color", glm::vec3(0.16f, 0.45f, 0.2f));

	Drawable::m_render_program.bind();
	
	m_crystals_vao.bind();
	glDrawElements(GL_TRIANGLES, m_crystals_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr);

	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	
	glDisable(GL_BLEND);
	if (culled) {
		glEnable(GL_CULL_FACE);
	}
	if (depth_tested) {
		glEnable(GL_DEPTH_TEST);
	}
}

bool CrystalTile::is_selected() const {
	return m_destructible_rock_tile.is_selected();
}

void CrystalTile::unselect() {
	m_destructible_rock_tile.unselect();
}

void CrystalTile::select(const glm::vec3& color) {
	m_destructible_rock_tile.select(color);
}

void CrystalTile::unhover() {
	m_destructible_rock_tile.unhover();
}

void CrystalTile::hover(const glm::vec3& color) {
	m_destructible_rock_tile.hover(color);
}

void CrystalTile::set_cliff_type(unsigned short cliff_type) {
	m_destructible_rock_tile.set_cliff_type(cliff_type);
}

RockTile::CliffType CrystalTile::get_cliff_type() const {
	return m_destructible_rock_tile.get_cliff_type();
}

bool CrystalTile::is_rock() const {
	return true;
}

CrystalTile::Data::Data(const glm::vec3& position, const glm::vec3& normal)
	: position(position),
	normal(normal) {
}

CrystalTile::CrystalTile(const Map& map, unsigned int x, unsigned int y, const std::vector<CrystalTile::Data>& vertices, const RockTile::CliffType& cliff_type)
	: Attackable(glm::translate(glm::vec3(0.5F, 1.0F, 0.5F)), *this, 200.0F, 200.0F),
	Tile(map, x, y, RenderPrograms::get_render_program("unit")),
	m_vertices_vbo(vertices, GL_ARRAY_BUFFER),
	m_crystals_elements_vbo(ObjLoader::get_obj_elements("crystals", 0), GL_ELEMENT_ARRAY_BUFFER),
	m_crystals_vao(),
	m_destructible_rock_tile(map, x, y, cliff_type),
	m_crystal_tile_transformation(glm::mat4(glm::translate(glm::vec3(get_size() / 2.0f, 0.9f, get_size() / 2.0f))) * glm::mat4(glm::scale(glm::vec3(0.15f, 0.15f, 0.15f)))) {
	
	set_is_walkable(false);
	
	m_crystals_vao.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CrystalTile::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CrystalTile::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_crystals_elements_vbo.bind();
	
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}
