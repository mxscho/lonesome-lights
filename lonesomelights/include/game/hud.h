#ifndef __GAME__HUD_H__
#define __GAME__HUD_H__

#include <glm/glm.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "game/game.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

class Camera;
class Timer;

class HUD : public Drawable, public Updatable {
public:
	HUD(Game& game, sf::RenderWindow& render_window, sf::VideoMode& video_mode);
	
	std::pair<bool, unsigned int> get_clicked_index(float mouse_x, float mouse_y);

	void update(const Timer& timer) override final;

	void draw(const Camera& camera) const override final;
private:
	Game& m_game;
	sf::RenderWindow& m_render_window;
	VertexBufferObject<glm::vec2> m_vbo;
	VertexArrayObject m_vao;

	sf::Font m_font;
	sf::Text m_plasma_count_text;
	sf::Text m_crystal_count_text;
};

#endif
