AM_CPPFLAGS = -I$(top_srcdir)/include

bin_PROGRAMS = $(top_builddir)/bin/lonesomelights_server $(top_builddir)/bin/lonesomelights_client
__top_builddir__bin_lonesomelights_SOURCES = \
	timer.cpp \
	updatable.cpp \
		\
		game/attackable.cpp \
		game/game.cpp \
		game/hud.cpp \
		game/player.cpp \
		game/player_handler.cpp \
			\
			game/map/map.cpp \
			game/map/base_tile.cpp \
			game/map/crystal_tile.cpp \
			game/map/destructible_rock_tile.cpp \
			game/map/floor_tile.cpp \
			game/map/indestructible_rock_tile.cpp \
			game/map/rock_tile.cpp \
			game/map/tile.cpp \
			\
			game/units/unit.cpp \
			game/units/crystal.cpp \
			game/units/laser_unit.cpp \
			game/units/selection_circle.cpp \
			game/units/shockwave.cpp \
			game/units/shockwave_unit.cpp \
			game/units/worker_unit.cpp \
			\
		geometry/camera.cpp \
		geometry/constant_accelerating_movement.cpp \
		geometry/constant_movement.cpp \
		geometry/inertial_movable.cpp \
		geometry/map_camera.cpp \
		geometry/movement.cpp \
		geometry/path_finder.cpp \
		geometry/perspective_camera.cpp \
		geometry/transformable.cpp \
		\
		networking/client.cpp \
		networking/participant.cpp \
		networking/network_handler.cpp \
		networking/network_id.cpp \
		networking/networkable.cpp \
		networking/network_packet.cpp \
		networking/server.cpp \
			\
			networking/network_handlers/client_handler.cpp \
			networking/network_handlers/server_handler.cpp \
				\
				networking/network_handlers/client_handlers/unit_client_handler.cpp \
				networking/network_handlers/server_handlers/unit_server_handler.cpp \
				\
		rendering/deferred_drawable.cpp \
		rendering/drawable.cpp \
			\
			rendering/opengl/fragment_shader.cpp \
			rendering/opengl/frame_buffer_object.cpp \
			rendering/opengl/render_program.cpp \
			rendering/opengl/render_programs.cpp \
			rendering/opengl/shader.cpp \
			rendering/opengl/texture.cpp \
			rendering/opengl/textures.cpp \
			rendering/opengl/vertex_array_object.cpp \
			rendering/opengl/vertex_buffer_objects.cpp \
			rendering/opengl/vertex_shader.cpp \
			\
			rendering/particles/health_bar_particle_emitter.cpp \
			rendering/particles/linear_particle_emitter.cpp \
			rendering/particles/particle.cpp \
			rendering/particles/particle_emitter.cpp \
			rendering/particles/spheric_particle_emitter.cpp \
				\
				rendering/particles/particle_systems/explosion.cpp \
				rendering/particles/particle_systems/laser.cpp \
				\
			rendering/obj_loader/obj_loader.cpp \
			rendering/obj_loader/tiny_obj_loader.cpp

__top_builddir__bin_lonesomelights_server_SOURCES = $(__top_builddir__bin_lonesomelights_SOURCES) \
	server_main.cpp
__top_builddir__bin_lonesomelights_client_SOURCES = $(__top_builddir__bin_lonesomelights_SOURCES) \
	client_main.cpp

__top_builddir__bin_lonesomelights_CPPFLAGS = -I$(top_srcdir)/include $(SFML_CPPFLAGS)
__top_builddir__bin_lonesomelights_server_CPPFLAGS = $(__top_builddir__bin_lonesomelights_CPPFLAGS)
__top_builddir__bin_lonesomelights_client_CPPFLAGS = $(__top_builddir__bin_lonesomelights_CPPFLAGS)
__top_builddir__bin_lonesomelights_CXXFLAGS = -std=c++11 -pedantic-errors -Wall -Werror -g
__top_builddir__bin_lonesomelights_server_CXXFLAGS = $(__top_builddir__bin_lonesomelights_CXXFLAGS)
__top_builddir__bin_lonesomelights_client_CXXFLAGS = $(__top_builddir__bin_lonesomelights_CXXFLAGS)

__top_builddir__bin_lonesomelights_LDADD = -lpng -lGL -lGLEW $(SFML_LIBS)
#-lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
__top_builddir__bin_lonesomelights_server_LDADD = $(__top_builddir__bin_lonesomelights_LDADD)
__top_builddir__bin_lonesomelights_client_LDADD = $(__top_builddir__bin_lonesomelights_LDADD)
