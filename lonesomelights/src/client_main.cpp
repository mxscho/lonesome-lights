#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <GL/glew.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "networking/client.h"

// TEST
#include "timer.h"
#include "game/units/laser_unit.h"
#include "networking/network_handlers/client_handlers/unit_client_handler.h"
#include <SFML/Window.hpp>
#include "rendering/opengl/vertex_shader.h"
#include "rendering/opengl/fragment_shader.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "game/map/map.h"
#include "rendering/particles/spheric_particle_emitter.h"
#include "geometry/transformable.h"
#include "geometry/map_camera.h"
#include "geometry/path_finder.h"
#include "rendering/obj_loader/obj_loader.h"

static sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();

static void print_usage_and_die(int argc, char** argv) {
	std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
	exit(EXIT_FAILURE);
}

static std::string get_args_host(int argc, char** argv) {
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

static unsigned int get_args_port(int argc, char** argv) {
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

static std::pair<bool, glm::vec3> get_clicked_world_position(const Camera& camera, int mouse_x, int mouse_y) {
	// Calculate world coordinates of z-buffer-top-most object which is drawn at the clicked screen coordinates.
	// To get the depth of the clicked pixel, we read depths in a 15x15 pixel square around this pixel.
	// If there's no valid depth and only background, we don't want this click to do anything.
	// The first valid depth we find (going from center to edges) will be the one used for getting mouse coordinates in world space.
	std::array<float, 15 * 15> depths;
	glReadPixels(
		mouse_x - 7,
		video_mode.height - mouse_y - 7,
		15,
		15,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		depths.data()
	);
	
	float depth = 0.0F;
	// Walks through the square and looks at depths from center to edges.
	for (int i_delta = 0; i_delta <= 7; ++i_delta) {
		for (int i_x = -i_delta; i_x <= i_delta; ++i_x) {
			for (int i_y = -i_delta; i_y <= i_delta; ++i_y) {
				if ((i_x == -i_delta || i_x == i_delta) &&
					(i_y == -i_delta || i_y == i_delta)) {
					float new_depth = depths[15 * (7 + i_y) + (7 + i_x)];
					if (new_depth != 0.0F && new_depth != 1.0F) {
						depth = new_depth;
						break;
					}
				}
			}
			if (depth != 0.0F) {
				break;
			}
		}
		if (depth != 0.0F) {
			break;
		}
	}
	if (depth == 0.0F) {
		return std::make_pair(false, glm::vec3());
	}

	glm::vec4 position(
		(float) mouse_x / video_mode.width * 2.0F - 1.0F,
		(1.0 - (float) mouse_y / video_mode.height) * 2.0F - 1.0F,
		depth * 2.0F - 1.0F,
		1.0F
	);

	position = glm::inverse(camera.get_view_transformation()) * glm::inverse(camera.get_projection_transformation()) * position;
	position /= position.w;
	return std::make_pair(true, glm::vec3(position.x, position.y, position.z));
}

int main(int argc, char** argv) {
	// TEST
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	sf::Window window(video_mode, "Lonesome Lights", sf::Style::Default, settings);
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
	
	Map map = Map::create_test_map(1.0F);
	PathFinder path_finder(map, 12);
	
	SphericParticleEmitter spheric_particle_emitter(
		glm::translate(glm::vec3(8.0F, 3.0F, 8.0F)), // Transformation
		map, // Parent transformable
		0.05F, // Billboard size
		timer.get_current_time_seconds(), // Current time (seconds)
		0.0F, // Radius
		0.5F, // Particle start velocity
		0.0F, // Particle acceleration
		5.0F, // Minimum particle lifetime (seconds)
		8.0F, // Maximum particle lifetime (seconds)
		200.0F  // Frequency
	);
	
	std::unique_ptr<LaserUnit> laser_unit = LaserUnit::create(glm::vec2(1.0F, 1.0F), map);
	/*UnitClientHandler unit_client_handler(client.create_base_network_id(), client);
	laser_unit.set_network_handler(unit_client_handler);*/
	
	MapCamera map_camera(map, glm::vec2(0.0F, 0.0F), (float) video_mode.width / video_mode.height);
	
	
	
	
	
	// --------
	
	VertexBufferObject<GLfloat> positions_vbo(ObjLoader::get_obj_positions("unit_0", 0), GL_ARRAY_BUFFER);
	VertexBufferObject<GLuint> elements_vbo(ObjLoader::get_obj_elements("unit_0", 0), GL_ELEMENT_ARRAY_BUFFER);
	VertexArrayObject vao;
	vao.bind();
	positions_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	elements_vbo.bind();
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
	
	// --------	
	
	
	
	
	
	
	
	
	
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::MouseWheelMoved) {
				map_camera.change_zoom(event.mouseWheel.delta);
			} else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Right) {
					std::pair<bool, glm::vec3> world_position = get_clicked_world_position(map_camera, event.mouseButton.x, event.mouseButton.y);
					if (world_position.first) {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
							laser_unit->add_target_position_to_path(timer, glm::vec2(world_position.second.x, world_position.second.z));
						} else {
							std::list<glm::vec2> path = path_finder.get_shortest_path(laser_unit->get_position_vec2(), glm::vec2(world_position.second.x, world_position.second.z), false);
							laser_unit->set_target_path(timer, path);
						}
					}
				}
			}
		}
		sf::Vector2i mouse_coordinates = sf::Mouse::getPosition(window);
		glm::vec2 mouse_position((float) mouse_coordinates.x / video_mode.width * 2.0F - 1.0F, (1.0F - (float) mouse_coordinates.y / video_mode.height) * 2.0F - 1.0F);
		if (std::abs(mouse_position.x) > 0.95F || std::abs(mouse_position.y) > 0.95F) {
			mouse_position = glm::normalize(mouse_position);
			map_camera.set_velocity(mouse_position * 10.0F);
		} else {
			map_camera.set_velocity(glm::vec2(0.0F));
		}
		
		static bool is_wireframe_mode = false;
		static bool is_wireframe_key_down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			if (!is_wireframe_key_down) {
				is_wireframe_mode = !is_wireframe_mode;
			}
			is_wireframe_key_down = true;
		}
		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			is_wireframe_key_down = false;
		}
		if (is_wireframe_mode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
		//glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		timer.advance();
		
		map_camera.update(timer);
		client.update();
		
		map.update(timer);
		spheric_particle_emitter.update(timer);
		laser_unit->update(timer);
		
		spheric_particle_emitter.draw(map_camera);
		map.draw(map_camera);
		laser_unit->draw(map_camera);
		
		// ----------
		
		const RenderProgram& render_program = RenderPrograms::get_render_program("unit");

		render_program.set_uniform("u_model_transformation", glm::mat4());
		render_program.set_uniforms("u_view_transformation", "u_projection_transformation", "u_camera_eye_position", "u_camera_up_direction", map_camera);

		render_program.bind();
		vao.bind();
		
		//glDrawElements(GL_TRIANGLES, elements_vbo.get_size(), GL_UNSIGNED_INT, nullptr);
		
		VertexArrayObject::unbind_any();
		RenderProgram::unbind_any();
		
		// ----------

		window.display();
	}
	
    return EXIT_SUCCESS;
}