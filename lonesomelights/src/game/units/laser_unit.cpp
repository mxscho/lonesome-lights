#include "game/units/laser_unit.h"

#include "game/map/map.h"
#include "rendering/obj_loader/obj_loader.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/textures.h"
#include "timer.h"

#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

std::unique_ptr<LaserUnit> LaserUnit::create(const glm::vec2& position, const Map& map) {
	std::vector<GLfloat> vine_positions = ObjLoader::get_obj_positions("unit_0", 0);
	std::vector<GLfloat> vine_normals = ObjLoader::get_obj_normals("unit_0", 0);
	std::vector<GLfloat> ball_positions = ObjLoader::get_obj_positions("unit_0", 1);
	std::vector<GLfloat> ball_normals = ObjLoader::get_obj_normals("unit_0", 1);
	
	std::vector<LaserUnit::Data> vertices;
	std::vector<unsigned int> vertex_counts;
	for (unsigned int i = 0; i < vine_positions.size(); i += 3) {
		vertices.push_back(LaserUnit::Data(glm::vec3(vine_positions[i + 0], vine_positions[i + 1], vine_positions[i + 2]), glm::vec3(vine_normals[i + 0], vine_normals[i + 1], vine_normals[i + 2])));
	}
	vertex_counts.push_back(vine_positions.size() / 3);
	for (unsigned int i = 0; i < ball_positions.size(); i += 3) {
		vertices.push_back(LaserUnit::Data(glm::vec3(ball_positions[i + 0], ball_positions[i + 1], ball_positions[i + 2]), glm::vec3(ball_normals[i + 0], ball_normals[i + 1], ball_normals[i + 2])));
	}
	vertex_counts.push_back(ball_positions.size() / 3);
	
	return std::unique_ptr<LaserUnit>(new LaserUnit(position, map, vertices, vertex_counts));
}

void LaserUnit::draw(const Camera& camera) const {
	Drawable::draw(camera);

	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);

	Drawable::m_render_program.bind();
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * m_vine_transformation);
	m_vine_vao.bind();
	glDrawElements(GL_TRIANGLES, m_vine_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr);
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * m_ball_transformation);
	m_ball_vao.bind();
	glDrawElementsBaseVertex(GL_TRIANGLES, m_ball_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr, m_vertex_counts[0]);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	
	m_sparks.draw(camera);
}

void LaserUnit::update(const Timer& timer) {
	Unit::update(timer);
	
	m_sparks.update(timer);
	
	float scale_factor = 0.6F + (sin(timer.get_current_time_seconds() * 2.0F) + 1.0F) / 2.0F * 0.4F;
	m_ball_transformation = glm::scale(glm::vec3(scale_factor, scale_factor, scale_factor));
	float rotation_angle = timer.get_current_time_seconds();
	m_vine_transformation = glm::rotate(rotation_angle, glm::vec3(0.0F, 1.0F, 0.0F));
	
	// TEST
	// Networkable::on_update();
}

LaserUnit::Data::Data(const glm::vec3& position, const glm::vec3& normal)
	: position(position),
	normal(normal) {
}

LaserUnit::LaserUnit(const glm::vec2& position, const Map& map, const std::vector<LaserUnit::Data>& vertices, const std::vector<unsigned int>& vertex_counts)
	: Unit(glm::translate(glm::vec3(0.0F, 0.2F, 0.0F)) * glm::scale(glm::vec3(0.0075F, 0.0075F, 0.0075F)), position, map, 1.0F, 0.5F, 0.5F),
	m_vertices_vbo(vertices, GL_ARRAY_BUFFER),
	m_vertex_counts(vertex_counts),
	m_vine_elements_vbo(ObjLoader::get_obj_elements("unit_0", 0), GL_ELEMENT_ARRAY_BUFFER),
	m_ball_elements_vbo(ObjLoader::get_obj_elements("unit_0", 1), GL_ELEMENT_ARRAY_BUFFER),
	m_vine_vao(),
	m_ball_vao(),
	m_vine_transformation(),
	m_ball_transformation(),

	m_sparks(
		glm::translate(glm::vec3(0.0F, -0.1F, 0.0F)), // Transformation
		*this, // Parent transformable
		glm::vec2(0.1F, 0.1F), // Billboard size
		Textures::get_texture("particles/smoke"), // Texture
		glm::vec3(5.0F, 0.1F, 0.0F), // Start color
		glm::vec3(0.7F, 0.6F, 0.3F), // End color
		0.0F, // Radius
		15.0F, // Particle start velocity
		-0.3F, // Gravity
		1.0F, // Minimum particle lifetime (seconds)
		1.5F, // Maximum particle lifetime (seconds)
		10.0F  // Frequency
	) {
	
	m_vine_vao.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LaserUnit::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LaserUnit::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_vine_elements_vbo.bind();
	
	m_ball_vao.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LaserUnit::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LaserUnit::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_ball_elements_vbo.bind();
	
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}