#include "game/hud.h"

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"
#include <iostream>

HUD::HUD()
	: Drawable(RenderPrograms::get_render_program("hud")),
	m_vbo({ glm::vec2(-1.0F, -1.0F), glm::vec2(1.0F, -1.0F), glm::vec2(1.0F, 1.0F), glm::vec2(-1.0F, 1.0F) }, GL_ARRAY_BUFFER),
	m_vao()	{
	m_vao.bind();
	m_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
}

std::pair<bool, unsigned int> HUD::get_clicked_index(float mouse_x, float mouse_y) {
	if (mouse_y > 0.8898148148F && mouse_y < 0.9611111111F) {
		if (mouse_x > 0.1203125F + 0 * 0.1140625F && mouse_x < 0.1609375F + 0 * 0.1140625F) {
			return std::make_pair(true, 0);
		}
		if (mouse_x > 0.1203125F + 1 * 0.1140625F && mouse_x < 0.1609375F + 1 * 0.1140625F) {
			return std::make_pair(true, 1);
		}
		if (mouse_x > 0.1203125F + 2 * 0.1140625F && mouse_x < 0.1609375F + 2 * 0.1140625F) {
			return std::make_pair(true, 2);
		}

		if (mouse_x > 0.1203125F + 0.4182291667F + 0 * 0.1140625F && mouse_x < 0.1609375F + 0.4182291667F + 0 * 0.1140625F) {
			return std::make_pair(true, 3);
		}
		if (mouse_x > 0.1203125F + 0.4182291667F + 1 * 0.1140625F && mouse_x < 0.1609375F + 0.4182291667F + 1 * 0.1140625F) {
			return std::make_pair(true, 4);
		}
		if (mouse_x > 0.1203125F + 0.4182291667F + 2 * 0.1140625F && mouse_x < 0.1609375F + 0.4182291667F + 2 * 0.1140625F) {
			return std::make_pair(true, 5);
		}
	}
	return std::make_pair(false, 0);
}


void HUD::draw(const Camera& camera) const {
	Drawable::draw(camera);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const Texture& texture = Textures::get_texture("hud");
	texture.bind(GL_TEXTURE0);	
	Drawable::m_render_program.set_uniform("u_texture", 0);
	
	Drawable::m_render_program.bind();
	m_vao.bind();

	glDrawArrays(GL_QUADS, 0, 4);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();

	Texture::unbind_any(GL_TEXTURE0);

	glDisable(GL_BLEND);
}
