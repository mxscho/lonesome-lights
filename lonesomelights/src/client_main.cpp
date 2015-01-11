#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "networking/client.h"

// TEST
#include "timer.h"
#include "game/unit.h"
#include "networking/network_handlers/client_handlers/unit_client_handler.h"
#include <SFML/Window.hpp>
#include "rendering/opengl/vertex_shader.h"
#include "rendering/opengl/fragment_shader.h"
#include "rendering/opengl/render_program.h"
#include "game/map/map.h"
#include "rendering/particles/particle_emitter.h"
#include "geometry/transformable.h"

void print_usage_and_die(int argc, char** argv) {
	std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
	exit(EXIT_FAILURE);
}

std::string get_args_host(int argc, char** argv) {
	if (argc <= 1) {
		print_usage_and_die(argc, argv);
	}
	std::stringstream stringstream;
	stringstream << argv[1];
	std::string host;
	stringstream >> host;
	if (host.empty()) {
		print_usage_and_die(argc, argv);
	}
	return host;
}

unsigned int get_args_port(int argc, char** argv) {
	if (argc <= 2) {
		print_usage_and_die(argc, argv);
	}
	std::stringstream stringstream;
	stringstream << argv[2];
	unsigned int port;
	stringstream >> port;
	if (port < 1 || port > 65535) {
		print_usage_and_die(argc, argv);
	}
	return port;
}

int main(int argc, char** argv) {
	// TEST
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	sf::Window window(sf::VideoMode(800, 600), "Lonesome Lights", sf::Style::Default, settings);
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK) {
		std::cerr << "Rendering: Glew initialization failed." << std::endl;
		return EXIT_FAILURE;
	}

	std::string host = get_args_host(argc, argv);
	unsigned int port = get_args_port(argc, argv);
	Client client;
	
	if (!client.connect(host, port)) {
		return EXIT_FAILURE;
	}
	
	// TEST
	Timer timer;
	timer.advance();
	timer.advance();
	
	/*Unit unit;
	UnitClientHandler unit_client_handler(client.create_base_network_id(), client);
	unit.set_network_handler(unit_client_handler);*/
	
	VertexShader particle_emitter_vertex_shader;
	if (!particle_emitter_vertex_shader.load_from_file("res/shaders/particle_emitter_vertex.glsl")) {
		return EXIT_FAILURE;
	}
	FragmentShader particle_emitter_fragment_shader;
	if (!particle_emitter_fragment_shader.load_from_file("res/shaders/particle_emitter_fragment.glsl")) {
		return EXIT_FAILURE;
	}
	RenderProgram particle_emitter_render_program;
	if (!particle_emitter_render_program.link(particle_emitter_vertex_shader, particle_emitter_fragment_shader)) {
		return EXIT_FAILURE;
	}
	ParticleEmitter particle_emitter(glm::vec3(-0.5F, 0.0F, 0.0F), glm::vec3(0.25F, 0.5F, 0.0F), glm::vec3(0.0F, -0.25F, 0.0F), 4.5F, 4.5F, 4.0F, timer.get_current_time_seconds(), 50);
	
	VertexShader map_vertex_shader;
	if (!map_vertex_shader.load_from_file("res/shaders/map_vertex.glsl")) {
		return EXIT_FAILURE;
	}
	FragmentShader map_fragment_shader;
	if (!map_fragment_shader.load_from_file("res/shaders/map_fragment.glsl")) {
		return EXIT_FAILURE;
	}
	RenderProgram map_emitter_render_program;
	if (!map_emitter_render_program.link(map_vertex_shader, map_fragment_shader)) {
		return EXIT_FAILURE;
	}
	Map map(glm::mat4(), 1, 1);
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		timer.advance();
		client.update();
		map.update(timer);
		particle_emitter.update(timer);
		map.draw(map_emitter_render_program);
		particle_emitter.draw(particle_emitter_render_program);
		
		//unit.update(timer);

		window.display();
	}
	
    return EXIT_SUCCESS;
}