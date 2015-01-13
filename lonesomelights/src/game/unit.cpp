#include "game/unit.h"

#include "game/map/map.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

Unit::Unit(const glm::vec2& position, const Map& map, float max_velocity, float acceleration, float decceleration)
	: Drawable(RenderPrograms::get_render_program("unit")),
	Networkable(),
	InertialMovable(glm::mat4(glm::translate(glm::vec3(position.x, 0.1F, position.y))), map, max_velocity, acceleration, decceleration),
	m_vertices_vbo({ glm::vec3(-0.05F, 0.0F, -0.05F), glm::vec3(0.05F, 0.0F, -0.05F), glm::vec3(0.05, 0.0F, 0.05), glm::vec3(-0.05F, 0.0F, 0.05) }, GL_ARRAY_BUFFER),
	m_vertex_array_object()	{
	m_vertex_array_object.bind();
	m_vertices_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	const Texture& texture = Textures::get_texture("map");
	Drawable::m_render_program.set_uniform("u_texture", texture.get_id());
}

void Unit::set_target_position(const Timer& timer, const glm::vec2& target_position) {
	InertialMovable::set_target_position(timer, glm::vec3(target_position.x, 0.1F, target_position.y));
}

void Unit::draw(const Camera& camera) const {
	Drawable::draw(camera);

	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", camera);

	Drawable::m_render_program.bind();
	m_vertex_array_object.bind();

	glDrawArrays(GL_QUADS, 0, 4);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
}

void Unit::update(const Timer& timer) {
	InertialMovable::update(timer);
	
	// TODO: Update unit.
	
	// TEST
	Networkable::on_update();
}