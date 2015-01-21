#include "game/units/selection_circle.h"

#include "game/map/map.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/texture.h"
#include "rendering/opengl/textures.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <iostream>

SelectionCircle::SelectionCircle(const glm::mat4& transformation, const Map& map)
	: Drawable(RenderPrograms::get_render_program("selection_circle")),
	Transformable(transformation, map),
	m_selected_circle_vbo({ glm::vec2(-0.5F, -0.5F), glm::vec2(-0.5F, 0.5F), glm::vec2(0.5F, 0.5F), glm::vec2(0.5F, -0.5F) }, GL_ARRAY_BUFFER),
	m_selected_circle_vao() {
	
	m_selected_circle_vao.bind();
	m_selected_circle_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}

void SelectionCircle::draw(const Camera& camera) const {
	Drawable::draw(camera);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_texture", 0);
	
	Drawable::m_render_program.bind();
	m_selected_circle_vao.bind();
	
	const Texture& selected_circle_texture = Textures::get_texture("selection_circle");
	selected_circle_texture.bind(GL_TEXTURE0);
	
	glDrawArrays(GL_QUADS, 0, 4);
	
	Texture::unbind_any(GL_TEXTURE0);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	
	glDisable(GL_BLEND);
}