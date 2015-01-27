#include "game/hud.h"

#include <cmath>
#include <string>

#include "game/server_game.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"

HUD::HUD(ClientGame& game, sf::RenderWindow& render_window, sf::VideoMode& video_mode)
	: Drawable(RenderPrograms::get_render_program("hud")),
	m_game(game),
	m_render_window(render_window),
	m_vbo({ glm::vec2(-1.0F, -1.0F), glm::vec2(1.0F, -1.0F), glm::vec2(1.0F, 1.0F), glm::vec2(-1.0F, 1.0F) }, GL_ARRAY_BUFFER),
	m_vao(),

	m_font(),
	m_plasma_count_text(),
	m_crystal_count_text() {
	m_vao.bind();
	m_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
	
	m_font.loadFromFile("res/fonts/Lato-Regular.ttf");

	m_plasma_count_text.setFont(m_font);
	m_plasma_count_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_plasma_count_text.setColor(sf::Color(200, 200, 200));
	m_plasma_count_text.setStyle(sf::Text::Bold);
	m_plasma_count_text.setPosition(0.8640625F * video_mode.width, 0.0195F * video_mode.height);

	m_crystal_count_text.setFont(m_font);
	m_crystal_count_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_crystal_count_text.setColor(sf::Color(200, 200, 200));
	m_crystal_count_text.setStyle(sf::Text::Bold);
	m_crystal_count_text.setPosition((0.8640625F + 0.1036458333F) * video_mode.width, 0.0195F * video_mode.height);
	
	
	// unit costs
	m_worker_unit_plasma_cost_text.setFont(m_font);
	m_worker_unit_plasma_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_worker_unit_plasma_cost_text.setColor(sf::Color(200, 200, 200));
	m_worker_unit_plasma_cost_text.setStyle(sf::Text::Bold);
	m_worker_unit_plasma_cost_text.setPosition((0.225F) * video_mode.width, 0.9F * video_mode.height);
	m_worker_unit_plasma_cost_text.setString(std::to_string(static_cast<unsigned int>(floor(ServerGame::c_worker_unit_plasma_cost))));
	
	m_worker_unit_crystals_cost_text.setFont(m_font);
	m_worker_unit_crystals_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_worker_unit_crystals_cost_text.setColor(sf::Color(200, 200, 200));
	m_worker_unit_crystals_cost_text.setStyle(sf::Text::Bold);
	m_worker_unit_crystals_cost_text.setPosition((0.225F) * video_mode.width, 0.932F * video_mode.height);
	m_worker_unit_crystals_cost_text.setString(std::to_string(static_cast<unsigned int>(floor(ServerGame::c_worker_unit_crystals_cost))));
	
	m_laser_unit_plasma_cost_text.setFont(m_font);
	m_laser_unit_plasma_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_laser_unit_plasma_cost_text.setColor(sf::Color(200, 200, 200));
	m_laser_unit_plasma_cost_text.setStyle(sf::Text::Bold);
	m_laser_unit_plasma_cost_text.setPosition((0.34F) * video_mode.width, 0.9F * video_mode.height);
	m_laser_unit_plasma_cost_text.setString(std::to_string(static_cast<unsigned int>(floor(ServerGame::c_laser_unit_plasma_cost))));
	
	m_laser_unit_crystals_cost_text.setFont(m_font);
	m_laser_unit_crystals_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_laser_unit_crystals_cost_text.setColor(sf::Color(200, 200, 200));
	m_laser_unit_crystals_cost_text.setStyle(sf::Text::Bold);
	m_laser_unit_crystals_cost_text.setPosition((0.34F) * video_mode.width, 0.932F * video_mode.height);
	m_laser_unit_crystals_cost_text.setString(std::to_string(static_cast<unsigned int>(floor(ServerGame::c_laser_unit_crystals_cost))));
	
	m_shockwave_unit_plasma_cost_text.setFont(m_font);
	m_shockwave_unit_plasma_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_shockwave_unit_plasma_cost_text.setColor(sf::Color(200, 200, 200));
	m_shockwave_unit_plasma_cost_text.setStyle(sf::Text::Bold);
	m_shockwave_unit_plasma_cost_text.setPosition((0.455F) * video_mode.width, 0.9F * video_mode.height);
	m_shockwave_unit_plasma_cost_text.setString(std::to_string(static_cast<unsigned int>(floor(ServerGame::c_shockwave_unit_plasma_cost))));
	
	m_shockwave_unit_crystals_cost_text.setFont(m_font);
	m_shockwave_unit_crystals_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_shockwave_unit_crystals_cost_text.setColor(sf::Color(200, 200, 200));
	m_shockwave_unit_crystals_cost_text.setStyle(sf::Text::Bold);
	m_shockwave_unit_crystals_cost_text.setPosition((0.455F) * video_mode.width, 0.932F * video_mode.height);
	m_shockwave_unit_crystals_cost_text.setString(std::to_string(static_cast<unsigned int>(floor(ServerGame::c_shockwave_unit_crystals_cost))));
	
	// upgrade costs
	m_upgrade1_plasma_cost_text.setFont(m_font);
	m_upgrade1_plasma_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_upgrade1_plasma_cost_text.setColor(sf::Color(200, 200, 200));
	m_upgrade1_plasma_cost_text.setStyle(sf::Text::Bold);
	m_upgrade1_plasma_cost_text.setPosition((0.643F) * video_mode.width, 0.9F * video_mode.height);
	m_upgrade1_plasma_cost_text.setString("todo");
	
	m_upgrade1_crystals_cost_text.setFont(m_font);
	m_upgrade1_crystals_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_upgrade1_crystals_cost_text.setColor(sf::Color(200, 200, 200));
	m_upgrade1_crystals_cost_text.setStyle(sf::Text::Bold);
	m_upgrade1_crystals_cost_text.setPosition((0.643F) * video_mode.width, 0.932F * video_mode.height);
	m_upgrade1_crystals_cost_text.setString("todo");
	
	m_upgrade2_plasma_cost_text.setFont(m_font);
	m_upgrade2_plasma_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_upgrade2_plasma_cost_text.setColor(sf::Color(200, 200, 200));
	m_upgrade2_plasma_cost_text.setStyle(sf::Text::Bold);
	m_upgrade2_plasma_cost_text.setPosition((0.758F) * video_mode.width, 0.9F * video_mode.height);
	m_upgrade2_plasma_cost_text.setString("todo");
	
	m_upgrade2_crystals_cost_text.setFont(m_font);
	m_upgrade2_crystals_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_upgrade2_crystals_cost_text.setColor(sf::Color(200, 200, 200));
	m_upgrade2_crystals_cost_text.setStyle(sf::Text::Bold);
	m_upgrade2_crystals_cost_text.setPosition((0.758F) * video_mode.width, 0.932F * video_mode.height);
	m_upgrade2_crystals_cost_text.setString("todo");
	
	m_upgrade3_plasma_cost_text.setFont(m_font);
	m_upgrade3_plasma_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_upgrade3_plasma_cost_text.setColor(sf::Color(200, 200, 200));
	m_upgrade3_plasma_cost_text.setStyle(sf::Text::Bold);
	m_upgrade3_plasma_cost_text.setPosition((0.873F) * video_mode.width, 0.9F * video_mode.height);
	m_upgrade3_plasma_cost_text.setString("todo");
	
	m_upgrade3_crystals_cost_text.setFont(m_font);
	m_upgrade3_crystals_cost_text.setCharacterSize(static_cast<unsigned int>(round(0.0222222222F * video_mode.height)));
	m_upgrade3_crystals_cost_text.setColor(sf::Color(200, 200, 200));
	m_upgrade3_crystals_cost_text.setStyle(sf::Text::Bold);
	m_upgrade3_crystals_cost_text.setPosition((0.873F) * video_mode.width, 0.932F * video_mode.height);
	m_upgrade3_crystals_cost_text.setString("todo");
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

void HUD::update(const Timer& timer) {
	m_plasma_count_text.setString(std::to_string(static_cast<unsigned int>(floor(m_game.get_own_plasma_count()))));
	m_plasma_count_text.setOrigin(m_plasma_count_text.getLocalBounds().left + m_plasma_count_text.getLocalBounds().width, m_plasma_count_text.getLocalBounds().top);
	m_crystal_count_text.setString(std::to_string(static_cast<unsigned int>(floor(m_game.get_own_crystal_count()))));
	m_crystal_count_text.setOrigin(m_plasma_count_text.getLocalBounds().left + m_plasma_count_text.getLocalBounds().width, m_crystal_count_text.getLocalBounds().top);
	
	// unit costs text positions
	m_worker_unit_plasma_cost_text.setOrigin(m_worker_unit_plasma_cost_text.getLocalBounds().left + m_worker_unit_plasma_cost_text.getLocalBounds().width, m_worker_unit_plasma_cost_text.getLocalBounds().top);
	m_worker_unit_crystals_cost_text.setOrigin(m_worker_unit_crystals_cost_text.getLocalBounds().left + m_worker_unit_crystals_cost_text.getLocalBounds().width, m_worker_unit_crystals_cost_text.getLocalBounds().top);
	m_laser_unit_plasma_cost_text.setOrigin(m_laser_unit_plasma_cost_text.getLocalBounds().left + m_laser_unit_plasma_cost_text.getLocalBounds().width, m_laser_unit_plasma_cost_text.getLocalBounds().top);
	m_laser_unit_crystals_cost_text.setOrigin(m_laser_unit_crystals_cost_text.getLocalBounds().left + m_laser_unit_crystals_cost_text.getLocalBounds().width, m_worker_unit_crystals_cost_text.getLocalBounds().top);
	m_shockwave_unit_plasma_cost_text.setOrigin(m_shockwave_unit_plasma_cost_text.getLocalBounds().left + m_shockwave_unit_plasma_cost_text.getLocalBounds().width, m_shockwave_unit_plasma_cost_text.getLocalBounds().top);
	m_shockwave_unit_crystals_cost_text.setOrigin(m_shockwave_unit_crystals_cost_text.getLocalBounds().left + m_shockwave_unit_crystals_cost_text.getLocalBounds().width, m_shockwave_unit_crystals_cost_text.getLocalBounds().top);

	// upgrade costs text positions
	m_upgrade1_plasma_cost_text.setOrigin(m_upgrade1_plasma_cost_text.getLocalBounds().left + m_upgrade1_plasma_cost_text.getLocalBounds().width, m_upgrade1_plasma_cost_text.getLocalBounds().top);	
	m_upgrade1_crystals_cost_text.setOrigin(m_upgrade1_crystals_cost_text.getLocalBounds().left + m_upgrade1_crystals_cost_text.getLocalBounds().width, m_upgrade1_crystals_cost_text.getLocalBounds().top);
	m_upgrade2_plasma_cost_text.setOrigin(m_upgrade2_plasma_cost_text.getLocalBounds().left + m_upgrade2_plasma_cost_text.getLocalBounds().width, m_upgrade2_plasma_cost_text.getLocalBounds().top);	
	m_upgrade2_crystals_cost_text.setOrigin(m_upgrade2_crystals_cost_text.getLocalBounds().left + m_upgrade2_crystals_cost_text.getLocalBounds().width, m_upgrade2_crystals_cost_text.getLocalBounds().top);
	m_upgrade3_plasma_cost_text.setOrigin(m_upgrade3_plasma_cost_text.getLocalBounds().left + m_upgrade3_plasma_cost_text.getLocalBounds().width, m_upgrade3_plasma_cost_text.getLocalBounds().top);	
	m_upgrade3_crystals_cost_text.setOrigin(m_upgrade3_crystals_cost_text.getLocalBounds().left + m_upgrade3_crystals_cost_text.getLocalBounds().width, m_upgrade3_crystals_cost_text.getLocalBounds().top);
	
	// unit costs text colors
	if (m_game.get_own_plasma_count() >= ServerGame::c_worker_unit_plasma_cost) {
		m_worker_unit_plasma_cost_text.setColor(sf::Color(200, 200, 200));
	} else {
		m_worker_unit_plasma_cost_text.setColor(sf::Color::Red);
	}
	if (m_game.get_own_crystal_count() >= ServerGame::c_worker_unit_crystals_cost) {
		m_worker_unit_crystals_cost_text.setColor(sf::Color(200, 200, 200));
	} else {
		m_worker_unit_crystals_cost_text.setColor(sf::Color::Red);
	}
	if (m_game.get_own_plasma_count() >= ServerGame::c_laser_unit_plasma_cost) {
		m_laser_unit_plasma_cost_text.setColor(sf::Color(200, 200, 200));
	} else {
		m_laser_unit_plasma_cost_text.setColor(sf::Color::Red);
	}
	if (m_game.get_own_crystal_count() >= ServerGame::c_laser_unit_crystals_cost) {
		m_laser_unit_crystals_cost_text.setColor(sf::Color(200, 200, 200));
	} else {
		m_laser_unit_crystals_cost_text.setColor(sf::Color::Red);
	}
	if (m_game.get_own_plasma_count() >= ServerGame::c_shockwave_unit_plasma_cost) {
		m_shockwave_unit_plasma_cost_text.setColor(sf::Color(200, 200, 200));
	} else {
		m_shockwave_unit_plasma_cost_text.setColor(sf::Color::Red);
	}
	if (m_game.get_own_crystal_count() >= ServerGame::c_shockwave_unit_crystals_cost) {
		m_shockwave_unit_crystals_cost_text.setColor(sf::Color(200, 200, 200));
	} else {
		m_shockwave_unit_crystals_cost_text.setColor(sf::Color::Red);
	}
	// upgrade costs text colors
	//TODO
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

	// Draw text.

	m_render_window.pushGLStates();

	m_render_window.draw(m_plasma_count_text);
	m_render_window.draw(m_crystal_count_text);
	m_render_window.draw(m_worker_unit_plasma_cost_text);
	m_render_window.draw(m_worker_unit_crystals_cost_text);
	m_render_window.draw(m_laser_unit_plasma_cost_text);
	m_render_window.draw(m_laser_unit_crystals_cost_text);
	m_render_window.draw(m_shockwave_unit_plasma_cost_text);
	m_render_window.draw(m_shockwave_unit_crystals_cost_text);
	m_render_window.draw(m_upgrade1_plasma_cost_text);
	m_render_window.draw(m_upgrade1_crystals_cost_text);
	m_render_window.draw(m_upgrade2_plasma_cost_text);
	m_render_window.draw(m_upgrade2_crystals_cost_text);
	m_render_window.draw(m_upgrade3_plasma_cost_text);
	m_render_window.draw(m_upgrade3_crystals_cost_text);

	m_render_window.popGLStates();
}
