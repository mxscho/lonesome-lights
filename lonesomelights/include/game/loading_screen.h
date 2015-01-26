#ifndef __GAME__LOADING_SCREEN_H__
#define __GAME__LOADING_SCREEN_H__

#include <glm/glm.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "game/game.h"
#include "rendering/drawable.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

class Camera;

class LoadingScreen : public Drawable {
public:
	LoadingScreen(sf::RenderWindow& render_window, sf::VideoMode video_mode);

	void draw(const Camera& camera) const override final;
	void draw() const;
	void set_text(std::string text);
private:
	sf::RenderWindow& m_render_window;
	VertexBufferObject<glm::vec2> m_vbo;
	VertexArrayObject m_vao;

	sf::Font m_font;
	sf::Text m_text;
};

#endif
