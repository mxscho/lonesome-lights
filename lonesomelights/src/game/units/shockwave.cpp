#include "game/units/shockwave.h"

#include "game/map/map.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/texture.h"
#include "rendering/opengl/textures.h"
#include "timer.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <iostream>

Shockwave::Shockwave(const glm::mat4& transformation, const Map& map, const glm::vec3& color)
	: Drawable(RenderPrograms::get_render_program("shockwave")),
	Transformable(transformation, map),
	m_instance_index(m_instance_count),
	m_color(color),
	m_shockwave_vbo({ glm::vec2(-0.5F, -0.5F), glm::vec2(-0.5F, 0.5F), glm::vec2(0.5F, 0.5F), glm::vec2(0.5F, -0.5F) }, GL_ARRAY_BUFFER),
	m_shockwave_vao(),
	m_current_time_seconds() {
	
	m_shockwave_vao.bind();
	m_shockwave_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
	
	m_instance_count = (m_instance_count + 1) % 100;
}

void Shockwave::update(const Timer& timer) {
	m_current_time_seconds = timer.get_current_time_seconds();
}

void Shockwave::draw(const Camera& camera) const {
	Drawable::draw(camera);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_texture", 0);
	Drawable::m_render_program.set_uniform("u_color", m_color);
	
	Drawable::m_render_program.bind();
	m_shockwave_vao.bind();
	
	const Texture& selected_circle_texture = Textures::get_texture("shockwave");
	selected_circle_texture.bind(GL_TEXTURE0);
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * glm::translate(glm::vec3(0.0F, m_instance_index * 0.0002F, 0.0F)) * glm::rotate(m_current_time_seconds, glm::vec3(0.0F, 1.0F, 0.0F)));
	glDrawArrays(GL_QUADS, 0, 4);
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation() * glm::translate(glm::vec3(0.0F, m_instance_index * 0.0002F + 0.0001F, 0.0F)) * glm::rotate(-m_current_time_seconds, glm::vec3(0.0F, 1.0F, 0.0F)));
	glDrawArrays(GL_QUADS, 0, 4);
	
	Texture::unbind_any(GL_TEXTURE0);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();
	
	glDisable(GL_BLEND);
}

unsigned int Shockwave::m_instance_count = 0;