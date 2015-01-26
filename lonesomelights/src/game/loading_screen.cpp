#include "game/loading_screen.h"

#include <cmath>
#include <string>

#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"

LoadingScreen::LoadingScreen(sf::RenderWindow& render_window, sf::VideoMode video_mode)
	: Drawable(RenderPrograms::get_render_program("hud")),
	m_render_window(render_window),
	m_vbo({ glm::vec2(-1.0F, -1.0F), glm::vec2(1.0F, -1.0F), glm::vec2(1.0F, 1.0F), glm::vec2(-1.0F, 1.0F) }, GL_ARRAY_BUFFER),
	m_vao(),
	m_font(),
	m_text() {
	
	m_vao.bind();
	m_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
	
	m_font.loadFromFile("res/fonts/Lato-Regular.ttf");

	m_text.setFont(m_font);
	m_text.setCharacterSize(static_cast<unsigned int>(round(0.07F * video_mode.height)));
	m_text.setColor(sf::Color::White);
	m_text.setStyle(sf::Text::Bold);
	m_text.setPosition(0.5F * video_mode.width, 0.5F * video_mode.height);
}

void LoadingScreen::draw(const Camera& camera) const {
}

void LoadingScreen::draw() const {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const Texture& texture = Textures::get_texture("loading_screen");
	texture.bind(GL_TEXTURE0);	
	Drawable::m_render_program.set_uniform("u_texture", 0);
	
	Drawable::m_render_program.bind();
	m_vao.bind();

	glDrawArrays(GL_QUADS, 0, 4);
	
	VertexArrayObject::unbind_any();
	RenderProgram::unbind_any();

	Texture::unbind_any(GL_TEXTURE0);

	glDisable(GL_BLEND);

	// Draw text.

	m_render_window.pushGLStates();
	m_render_window.draw(m_text);
	m_render_window.popGLStates();
}


void LoadingScreen::set_text(std::string text) {
	m_text.setString(text);
	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2.0F, m_text.getLocalBounds().top);
}
