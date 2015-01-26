#include <cstdlib>
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "timer.h"

#include "game/game.h"

#include "networking/server.h"
#include "networking/network_handlers/server_handlers/game_server_handler.h"

//static sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
//static unsigned int style = sf::Style::Fullscreen | sf::Style::Close;
static sf::VideoMode video_mode = sf::VideoMode(200, 100);
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

	Timer timer;
	Game game;
	GameServerHandler game_server_handler(server.create_base_network_id(), server);
	game.set_network_handler(game_server_handler);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
			}
		}

		timer.advance();

		server.update();
		if (server.get_participants().size() < 1) {
			continue;
		}

		game.update(timer);
	}
    return EXIT_SUCCESS;
}
