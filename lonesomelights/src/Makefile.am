AM_CPPFLAGS = -I$(top_srcdir)/include

bin_PROGRAMS = $(top_builddir)/bin/lonesomelights_server $(top_builddir)/bin/lonesomelights_client
__top_builddir__bin_lonesomelights_SOURCES = \
	timer.cpp \
	updatable.cpp \
		\
		game/game_object.cpp \
			\
			game/game_objects/unit.cpp \
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
		rendering/drawable.cpp \
			\
			rendering/opengl/fragment_shader.cpp \
			rendering/opengl/frame_buffer_object.cpp \
			rendering/opengl/render_program.cpp \
			rendering/opengl/shader.cpp \
			rendering/opengl/vertex_array_object.cpp \
			rendering/opengl/vertex_buffer_objects.cpp \
			rendering/opengl/vertex_shader.cpp \
			\
			rendering/particles/particle.cpp \
			rendering/particles/particle_emitter.cpp

__top_builddir__bin_lonesomelights_server_SOURCES = $(__top_builddir__bin_lonesomelights_SOURCES) \
	server_main.cpp
__top_builddir__bin_lonesomelights_client_SOURCES = $(__top_builddir__bin_lonesomelights_SOURCES) \
	client_main.cpp

__top_builddir__bin_lonesomelights_CPPFLAGS = -I$(top_srcdir)/include
__top_builddir__bin_lonesomelights_server_CPPFLAGS = $(__top_builddir__bin_lonesomelights_CPPFLAGS)
__top_builddir__bin_lonesomelights_client_CPPFLAGS = $(__top_builddir__bin_lonesomelights_CPPFLAGS)
__top_builddir__bin_lonesomelights_CXXFLAGS = -std=c++11 -pedantic-errors -Wall -Werror -g
__top_builddir__bin_lonesomelights_server_CXXFLAGS = $(__top_builddir__bin_lonesomelights_CXXFLAGS)
__top_builddir__bin_lonesomelights_client_CXXFLAGS = $(__top_builddir__bin_lonesomelights_CXXFLAGS)

__top_builddir__bin_lonesomelights_LDADD = -lGL -lGLEW -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
__top_builddir__bin_lonesomelights_server_LDADD = $(__top_builddir__bin_lonesomelights_LDADD)
__top_builddir__bin_lonesomelights_client_LDADD = $(__top_builddir__bin_lonesomelights_LDADD)
