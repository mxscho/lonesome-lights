#include <cstdlib>
#include <iostream>
#include <sstream>

#include "networking/server.h"

// TEST
#include "timer.h"
#include "game/unit.h"
#include "networking/network_handlers/server_handlers/unit_server_handler.h"

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
	// TEST
	Timer timer;
	//Unit unit;
	//UnitServerHandler unit_server_handler(server.create_base_network_id(), server);
	//unit.set_network_handler(unit_server_handler);
	while (true) {
		server.update();
		if (server.get_participants().size() == 0) {
			continue;
		}
		
		//unit.update(timer);
	}
    return EXIT_SUCCESS;
}
