#include <cstdlib>
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "timer.h"
#include "game/loading_screen.h"
#include "game/server_game.h"

#include "networking/server.h"
#include "networking/network_handlers/server_handlers/game_server_handler.h"

//static sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
//static unsigned int style = sf::Style::Fullscreen | sf::Style::Close;
static sf::VideoMode video_mode = sf::VideoMode(500, 375);
static unsigned int style = sf::Style::Titlebar | sf::Style::Close;

void print_usage_and_die(int argc, char** argv) {
	std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
	exit(EXIT_FAILURE);
}

unsigned int get_args_port(int argc, char** argv) {
	if (argc <= 1) {
		print_usage_and_die(argc, argv);
	}
	std::stringstream stringstream;
	stringstream << argv[1];
	unsigned int port;
	stringstream >> port;
	if (port < 1 || port > 65535) {
		print_usage_and_die(argc, argv);
	}
	return port;
}

static void draw_loading_screen(sf::RenderWindow& window, LoadingScreen& loading_screen, std::string text) {
	loading_screen.set_text(text);
	loading_screen.draw();
}

int main(int argc, char** argv) {
	unsigned int port = get_args_port(argc, argv);
	Server server;
	
	if (!server.start(port)) {
		return EXIT_FAILURE;
	}

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	sf::RenderWindow window(video_mode, "Lonesome Lights (Server)", style, settings);
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK) {
		std::cerr << "Rendering: Glew initialization failed." << std::endl;
		return EXIT_FAILURE;
	}

	LoadingScreen loading_screen(window, video_mode);

	Timer timer;
	ServerGame game(server);
	GameServerHandler game_server_handler(server.create_base_network_id(), server);
	game.set_network_handler(game_server_handler);
	timer.reset(0.0F);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				break;
			}
		}

		server.update();
		if (!server.is_listening()) return EXIT_SUCCESS;

		draw_loading_screen(window, loading_screen, "Server is running...");
		loading_screen.draw();
		window.display();

		static bool started = false;
		if (server.get_participants().size() < 2) {
			if (!started) {
				timer.reset(0.0F);
				continue;
			} else if (server.get_participants().size() == 0) {
				return EXIT_SUCCESS;
			}
		}
		started = true;

		timer.advance();

		game.update(timer);
	}
    return EXIT_SUCCESS;
}
