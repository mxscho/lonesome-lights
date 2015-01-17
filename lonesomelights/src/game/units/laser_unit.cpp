#include "game/units/laser_unit.h"

#include "game/map/map.h"
#include "rendering/obj_loader/obj_loader.h"
#include "rendering/opengl/render_program.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <iostream>

std::unique_ptr<LaserUnit> LaserUnit::create(const glm::vec2& position, const Map& map) {
	std::vector<GLfloat> positions_0 = ObjLoader::get_obj_positions("unit_0", 0);
	std::vector<GLfloat> normals_0 = ObjLoader::get_obj_normals("unit_0", 0);
	std::vector<GLuint> elements_0 = ObjLoader::get_obj_elements("unit_0", 0);
	std::vector<GLfloat> positions_1 = ObjLoader::get_obj_positions("unit_0", 1);
	std::vector<GLfloat> normals_1 = ObjLoader::get_obj_normals("unit_0", 1);
	std::vector<GLuint> elements_1 = ObjLoader::get_obj_elements("unit_0", 1);
	
	std::vector<LaserUnit::Data> vertices;
	for (unsigned int i = 0; i < positions_0.size(); i += 3) {
		vertices.push_back(LaserUnit::Data(glm::vec3(positions_0[i + 0], positions_0[i + 1], positions_0[i + 2]), glm::vec3(normals_0[i + 0], normals_0[i + 1], normals_0[i + 2])));
	}
	for (unsigned int i = 0; i < positions_1.size(); i += 3) {
		vertices.push_back(LaserUnit::Data(glm::vec3(positions_1[i + 0], positions_1[i + 1], positions_1[i + 2]), glm::vec3(normals_1[i + 0], normals_1[i + 1], normals_1[i + 2])));
	}
	std::vector<GLuint> elements;
	elements.insert(elements.end(), elements_0.begin(), elements_0.end());
	for (unsigned int i = 0; i < elements_1.size(); ++i) {
		elements.push_back(positions_0.size() / 3 + elements_1[i]);
	}
	
	return std::unique_ptr<LaserUnit>(new LaserUnit(position, map, vertices, elements));
}

void LaserUnit::draw(const Camera& camera) const {
	Drawable::draw(camera);

	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);

	Drawable::m_render_program.bind();
	m_vertex_array_object.bind();

	glDrawElements(GL_TRIANGLES, m_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
}

void LaserUnit::update(const Timer& timer) {
	Unit::update(timer);
	
	// TODO: Update unit.
	
	// TEST
	// Networkable::on_update();
}

LaserUnit::Data::Data(const glm::vec3& position, const glm::vec3& normal)
	: position(position),
	normal(normal) {
}

LaserUnit::LaserUnit(const glm::vec2& position, const Map& map, const std::vector<LaserUnit::Data>& vertices, const std::vector<GLuint>& elements)
	: Unit(glm::translate(glm::vec3(0.0F, 0.2F, 0.0F)) * glm::scale(glm::vec3(0.01F, 0.01F, 0.01F)), position, map, 1.0F, 0.5F, 0.5F),
	m_vertices_vbo(vertices, GL_ARRAY_BUFFER),
	m_elements_vbo(elements, GL_ELEMENT_ARRAY_BUFFER),
	m_vertex_array_object()	{
	m_vertex_array_object.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LaserUnit::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LaserUnit::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_elements_vbo.bind();
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}