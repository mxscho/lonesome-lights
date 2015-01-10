#include "networking/network_handlers/server_handler.h"

ServerHandler::ServerHandler(const NetworkID& network_id, Server& server)
	: NetworkHandler(network_id),
	m_server(server) {
}