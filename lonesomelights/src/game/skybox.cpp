#include "game/skybox.h"

#include "game/map/map.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/texture.h"
#include "rendering/opengl/textures.h"
#include "timer.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <iostream>

Skybox::Skybox(const glm::mat4& transformation, const Map& map)
	: Drawable(RenderPrograms::get_render_program("skybox")),
	Transformable(transformation, map),
	m_up_vbo({ Skybox::Data(glm::vec3(-0.5F, 0.5F, -0.5F), glm::vec2(0.0F, 0.0F)),
			Skybox::Data(glm::vec3(-0.5F, 0.5F,  0.5F), glm::vec2(0.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, 0.5F,  0.5F), glm::vec2(1.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, 0.5F,  -0.5F), glm::vec2(1.0F, 0.0F)) }, GL_ARRAY_BUFFER),
	m_up_vao(),
	m_bottom_vbo({ Skybox::Data(glm::vec3(-0.5F, -0.5F, -0.5F), glm::vec2(0.0F, 0.0F)),
			Skybox::Data(glm::vec3(-0.5F, -0.5F,  0.5F), glm::vec2(0.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, -0.5F,  0.5F), glm::vec2(1.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, -0.5F,  -0.5F), glm::vec2(1.0F, 0.0F)) }, GL_ARRAY_BUFFER),
	m_bottom_vao(),
	m_left_vbo({ Skybox::Data(glm::vec3(-0.5F, -0.5F, -0.5F), glm::vec2(0.0F, 0.0F)),
			Skybox::Data(glm::vec3(-0.5F, -0.5F,  0.5F), glm::vec2(0.0F, 1.0F)),
			Skybox::Data(glm::vec3(-0.5F, 0.5F,  0.5F), glm::vec2(1.0F, 1.0F)),
			Skybox::Data(glm::vec3(-0.5F, 0.5F,  -0.5F), glm::vec2(1.0F, 0.0F)) }, GL_ARRAY_BUFFER),
	m_left_vao(),
	m_right_vbo({ Skybox::Data(glm::vec3(0.5F, -0.5F, -0.5F), glm::vec2(0.0F, 0.0F)),
			Skybox::Data(glm::vec3(0.5F, -0.5F,  0.5F), glm::vec2(0.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, 0.5F,  0.5F), glm::vec2(1.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, 0.5F,  -0.5F), glm::vec2(1.0F, 0.0F)) }, GL_ARRAY_BUFFER),
	m_right_vao(),
	m_front_vbo({ Skybox::Data(glm::vec3(-0.5F, -0.5F, -0.5F), glm::vec2(0.0F, 0.0F)),
			Skybox::Data(glm::vec3(-0.5F, 0.5F,  -0.5F), glm::vec2(0.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, 0.5F,  -0.5F), glm::vec2(1.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, -0.5F,  -0.5F), glm::vec2(1.0F, 0.0F)) }, GL_ARRAY_BUFFER),
	m_front_vao(),
	m_back_vbo({ Skybox::Data(glm::vec3(-0.5F, -0.5F, 0.5F), glm::vec2(0.0F, 0.0F)),
			Skybox::Data(glm::vec3(-0.5F, 0.5F,  0.5F), glm::vec2(0.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, 0.5F,  0.5F), glm::vec2(1.0F, 1.0F)),
			Skybox::Data(glm::vec3(0.5F, -0.5F,  0.5F), glm::vec2(1.0F, 0.0F)) }, GL_ARRAY_BUFFER),
	m_back_vao() {

	m_up_vao.bind();
	m_up_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), (void*) (3 * sizeof(float)));
	
	m_bottom_vao.bind();
	m_bottom_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), (void*) (3 * sizeof(float)));
	
	m_left_vao.bind();
	m_left_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), (void*) (3 * sizeof(float)));
	
	m_right_vao.bind();
	m_right_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), (void*) (3 * sizeof(float)));
	
	m_front_vao.bind();
	m_front_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), (void*) (3 * sizeof(float)));
	
	m_back_vao.bind();
	m_back_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Skybox::Data), (void*) (3 * sizeof(float)));

	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}

void Skybox::draw(const Camera& camera) const {
	Drawable::draw(camera);
	
	Drawable::m_render_program.set_uniform("u_model_transformation", Transformable::get_global_transformation());
	Drawable::m_render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", camera);
	Drawable::m_render_program.set_uniform("u_texture", 0);
	Drawable::m_render_program.set_uniform("u_color", glm::vec3(1.0F, 0.0F, 0.0F));
	
	Drawable::m_render_program.bind();
	
	glDisable(GL_CULL_FACE);
	m_up_vao.bind();
	const Texture& texture_up = Textures::get_texture("skybox/6");
	texture_up.bind(GL_TEXTURE0);
	glDrawArrays(GL_QUADS, 0, 4);
	
	m_bottom_vao.bind();
	const Texture& texture_bottom = Textures::get_texture("skybox/5");
	texture_bottom.bind(GL_TEXTURE0);
	glDrawArrays(GL_QUADS, 0, 4);
	
	m_left_vao.bind();
	const Texture& texture_left = Textures::get_texture("skybox/4");
	texture_left.bind(GL_TEXTURE0);
	glDrawArrays(GL_QUADS, 0, 4);
	
	m_right_vao.bind();
	const Texture& texture_right = Textures::get_texture("skybox/3");
	texture_right.bind(GL_TEXTURE0);
	glDrawArrays(GL_QUADS, 0, 4);
	
	m_front_vao.bind();
	const Texture& texture_front = Textures::get_texture("skybox/2");
	texture_front.bind(GL_TEXTURE0);
	glDrawArrays(GL_QUADS, 0, 4);
	
	m_back_vao.bind();
	const Texture& texture_back = Textures::get_texture("skybox/1");
	texture_back.bind(GL_TEXTURE0);
	glDrawArrays(GL_QUADS, 0, 4);
	
	Texture::unbind_any(GL_TEXTURE0);
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();

}

Skybox::Data::Data(const glm::vec3& position, const glm::vec2& texel)
	: position(position),
	texel(texel) {
}
