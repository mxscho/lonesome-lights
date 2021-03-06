#include <array>
#include <cassert>
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

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "timer.h"

#include "game/player_handler.h"
#include "game/client_game.h"
#include "game/units/laser_unit.h"
#include "game/map/map.h"
#include "geometry/transformable.h"
#include "geometry/map_camera.h"
#include "geometry/path_finder.h"
#include "game/player.h"
#include "game/hud.h"
#include "game/loading_screen.h"
#include "game/skybox.h"

#include "rendering/opengl/frame_buffer_object.h"
#include "rendering/obj_loader/obj_loader.h"
#include "rendering/opengl/textures.h"
#include "rendering/particles/particle_systems/explosion.h"
#include "rendering/particles/particle_systems/laser.h"
#include "rendering/opengl/vertex_shader.h"
#include "rendering/opengl/fragment_shader.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"

#include "networking/client.h"
#include "networking/network_handlers/client_handlers/game_client_handler.h"

static sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();
static unsigned int style = sf::Style::Fullscreen | sf::Style::Close;
//static sf::VideoMode video_mode = sf::VideoMode(1280, 800);
//static unsigned int style = sf::Style::Titlebar | sf::Style::Close;

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
	// Mouse is in HUD.
	if ((float) mouse_y / video_mode.height < 0.064F || (float) mouse_y / video_mode.height > 0.84F) {
		return std::make_pair(false, glm::vec3());
	}

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


static void draw_loading_screen(sf::RenderWindow& window, LoadingScreen& loading_screen, std::string text) {
	loading_screen.set_text(text);
	loading_screen.draw();
}

int main(int argc, char** argv) {std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	sf::RenderWindow window(video_mode, "Lonesome Lights", style, settings);
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK) {
		std::cerr << "Rendering: Glew initialization failed." << std::endl;
		return EXIT_FAILURE;
	}

	LoadingScreen loading_screen(window, video_mode);

	std::string host = get_args_host(argc, argv);
	unsigned int port = get_args_port(argc, argv);
	Client client;
	
	if (!client.connect(host, port)) {
		return EXIT_FAILURE;
	}

	Timer timer;
	timer.advance();
	
	ClientGame game(client);
	GameClientHandler game_client_handler(client.create_base_network_id(), client);
	game.set_network_handler(game_client_handler);

	Skybox skybox(glm::translate(glm::vec3(15.0F, 0.0F, 15.0F)) * glm::scale(glm::vec3(60.0F, 60.0F, 60.0F)), game.get_map());
	PlayerHandler player_handler(game);
	std::unique_ptr<MapCamera> map_camera;
	HUD hud(game, window, video_mode);

	FrameBufferObject frame_buffer_object;
	Texture color_texture;
	bool result = color_texture.generate_empty(video_mode.width, video_mode.height, GL_TEXTURE0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
	assert(result);
	Texture position_texture;
	result = position_texture.generate_empty(video_mode.width, video_mode.height, GL_TEXTURE1, GL_RGBA32F, GL_RGBA, GL_FLOAT);
	assert(result);
	Texture normal_texture;
	result = normal_texture.generate_empty(video_mode.width, video_mode.height, GL_TEXTURE2, GL_RGBA32F, GL_RGBA, GL_FLOAT);
	assert(result);
	Texture depth_texture;
	result = depth_texture.generate_empty(video_mode.width, video_mode.height, GL_TEXTURE3, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
	assert(result);
	
	frame_buffer_object.bind();
	frame_buffer_object.attach_colorbuffer_from_texture(color_texture);
	frame_buffer_object.attach_colorbuffer_from_texture(position_texture);
	frame_buffer_object.attach_colorbuffer_from_texture(normal_texture);
	frame_buffer_object.attach_depthbuffer_from_texture(depth_texture);
	frame_buffer_object.finalize_attaching();
	result = frame_buffer_object.check_status();
	assert(result);
	
	FrameBufferObject::unbind_any();
	
	VertexArrayObject fbo_vao;
	VertexBufferObject<glm::vec2> fbo_vbo({ glm::vec2(-1.0F, -1.0F), glm::vec2(1.0F, -1.0F), glm::vec2(1.0F, 1.0F), glm::vec2(-1.0F, 1.0F) }, GL_ARRAY_BUFFER);
	fbo_vao.bind();
	fbo_vbo.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	VertexArrayObject::unbind_any();
	VertexBufferObjects::unbind_any();
	
	timer.reset(0.0F);
	while (window.isOpen()) {
		sf::Vector2i mouse_coordinates = sf::Mouse::getPosition(window);
		std::pair<bool, glm::vec3> world_position = map_camera ? get_clicked_world_position(*map_camera, mouse_coordinates.x, mouse_coordinates.y) : std::make_pair(false, glm::vec3());
		glm::vec2 mouse_position((float) mouse_coordinates.x / video_mode.width * 2.0F - 1.0F, (1.0F - (float) mouse_coordinates.y / video_mode.height) * 2.0F - 1.0F);		
		
		if (world_position.first) {
			player_handler.on_mouse_hover(timer, world_position.second);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
			return EXIT_SUCCESS;
		}
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return EXIT_SUCCESS;
			} else if (event.type == sf::Event::MouseWheelMoved) {
				if (map_camera) {
					map_camera->change_zoom(event.mouseWheel.delta);
				}
			} else if (event.type == sf::Event::MouseButtonPressed) {
				if (world_position.first) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						player_handler.on_mouse_select(timer, world_position.second, true, false);
					} else if (event.mouseButton.button == sf::Mouse::Right) {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
							player_handler.on_mouse_select(timer, world_position.second, false, true);
						} else {
							player_handler.on_mouse_select(timer, world_position.second, false, false);
						}
					}
				}
				auto clicked_index = hud.get_clicked_index((float) mouse_coordinates.x / video_mode.width, (float) mouse_coordinates.y / video_mode.height);
				if (clicked_index.first) {
					if (clicked_index.second == 0) {
						game.spawn_own_worker_unit();
					} else if (clicked_index.second == 1) {
						game.spawn_own_laser_unit();
					} else if (clicked_index.second == 2) {
						game.spawn_own_shockwave_unit();
					} else if (clicked_index.second == 3) {
						game.upgrade_damage();
					} else if (clicked_index.second == 4) {
						game.upgrade_range();
					}
				}
			}
		}

		if (std::abs(mouse_position.x) > 0.95F || std::abs(mouse_position.y) > 0.95F) {
			mouse_position = glm::normalize(mouse_position);
			if (map_camera) {
				map_camera->set_velocity(mouse_position * 10.0F);
			}
		} else {
			if (map_camera) {
				map_camera->set_velocity(glm::vec2(0.0F));
			}
		}
		
		// Update scene.
		
		timer.advance();

		client.update();
		if (!client.is_connected()) return EXIT_SUCCESS;
		game.update(timer);

		if (!game.has_started()) {
			timer.reset(0.0F);
		} else {
			static bool first_time = true;
			if (first_time) {
				first_time = false;

				glm::vec2 camera_position = glm::vec2(0.0F, 0.0F);
				if (game.m_player_id == 0U) {
					camera_position = glm::vec2(static_cast<float>(ClientGame::c_own_base_x) - 3.0F, static_cast<float>(ClientGame::c_own_base_y) - 3.0F);
				} else {
					camera_position = glm::vec2(static_cast<float>(ClientGame::c_opponent_base_x) - 3.0F, static_cast<float>(ClientGame::c_opponent_base_y) - 3.0F);
				}
				map_camera = std::unique_ptr<MapCamera>(new MapCamera(game.get_map(), camera_position, (float) video_mode.width / video_mode.height));
			}

			map_camera->update(timer);
			hud.update(timer);

			// Settings for rendering.
		
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glEnable(GL_DEPTH_TEST);
		
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
		
			glClearColor(0.0F, 0.0F, 0.2F, 0.0F);
		
			// Render scene to scene texture.
		
			frame_buffer_object.bind();
		
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			game.draw(*map_camera);
		
			FrameBufferObject::unbind_any();
		}
		
		window.clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!game.has_started()) {
			draw_loading_screen(window, loading_screen, "Waiting for opponent...");
		} else {
			// Render scene texture to screen.

			const RenderProgram& render_program = RenderPrograms::get_render_program("deferred");
			render_program.set_uniform("u_color_texture", 0);
			render_program.set_uniform("u_position_texture", 1);
			render_program.set_uniform("u_normal_texture", 2);
			render_program.set_uniform("u_depth_texture", 3);

			render_program.bind();
			fbo_vao.bind();

			color_texture.bind(GL_TEXTURE0);
			position_texture.bind(GL_TEXTURE1);
			normal_texture.bind(GL_TEXTURE2);
			depth_texture.bind(GL_TEXTURE3);
		
			glDrawArrays(GL_QUADS, 0, 4);

			Texture::unbind_any(GL_TEXTURE0);
			Texture::unbind_any(GL_TEXTURE1);
			Texture::unbind_any(GL_TEXTURE2);
			Texture::unbind_any(GL_TEXTURE3);

			VertexArrayObject::unbind_any();
			RenderProgram::unbind_any();
		
			// Deferred rendering to screen.
		
			skybox.draw(*map_camera);
			game.draw_deferred(*map_camera, color_texture, position_texture, normal_texture, depth_texture);

			hud.draw(*map_camera);

			if (game.m_is_won) {
				if (game.m_won_id == 0) {
					draw_loading_screen(window, loading_screen, "Base has been destroyed!");
				} else if (game.m_won_id == 1) {
					draw_loading_screen(window, loading_screen, "Base has been destroyed!");
				}
			}
		}

		window.display();
	}
	
    return EXIT_SUCCESS;
}
