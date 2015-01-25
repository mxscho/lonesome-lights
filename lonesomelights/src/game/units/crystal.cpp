#include "game/units/crystal.h"

#include "game/map/map.h"
#include "rendering/obj_loader/obj_loader.h"
#include "rendering/opengl/render_programs.h"
#include "timer.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <iostream>

Crystal Crystal::create(const glm::mat4& transformation, const Map& map) {

	std::vector<GLfloat> crystal_positions = ObjLoader::get_obj_positions("crystal", 0);
	std::vector<GLfloat> crystal_normals = ObjLoader::get_obj_normals("crystal", 0);
	
	std::vector<Crystal::Data> vertices;
	for (unsigned int i = 0; i < crystal_positions.size(); i += 3) {
		vertices.push_back(Crystal::Data(glm::vec3(crystal_positions[i + 0], crystal_positions[i + 1], crystal_positions[i + 2]), glm::vec3(crystal_normals[i + 0], crystal_normals[i + 1], crystal_normals[i + 2])));
	}
	
	return Crystal(transformation, map, vertices);
}

void Crystal::update(const Timer& timer) {
	m_current_time_seconds = timer.get_current_time_seconds();
}

void Crystal::draw(const Camera& camera) const {
	Drawable::draw(camera);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	bool depth_test = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * glm::rotate(m_current_time_seconds, glm::vec3(0.0F, 1.0F, 0.0F)));
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_color", m_color);
	
	Drawable::m_render_program.bind();
	m_crystal_vao.bind();
	
	glDrawElements(GL_TRIANGLES, m_crystal_elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	
	if (depth_test) {
		glEnable(GL_DEPTH_TEST);
	}
	glDisable(GL_BLEND);
}

Crystal::Data::Data(const glm::vec3& position, const glm::vec3& normal)
	: position(position),
	normal(normal) {
}

Crystal::Crystal(const glm::mat4& transformation, const Map& map, const std::vector<Crystal::Data>& vertices)
	: Drawable(RenderPrograms::get_render_program("unit")),
	Transformable(transformation, map),
	m_color(glm::vec3(0.16f, 0.45f, 0.2f)),
	m_crystal_vbo(vertices, GL_ARRAY_BUFFER),
	m_crystal_elements_vbo(ObjLoader::get_obj_elements("crystal", 0), GL_ELEMENT_ARRAY_BUFFER),
	m_crystal_vao(),
	m_current_time_seconds(0.0f) {

	m_crystal_vao.bind();
	m_crystal_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Crystal::Data), (void*) ((0 * 3) * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Crystal::Data), (void*) ((1 * 3) * sizeof(GL_FLOAT)));
	m_crystal_elements_vbo.bind();

	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}
