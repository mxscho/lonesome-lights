#include "networking/network_handlers/client_handlers/game_client_handler.h"

#include <string>

#include "game/game.h"

#include "networking/client.h"
#include "networking/network_packet.h"
#include <iostream>

GameClientHandler::GameClientHandler(const NetworkID& network_id, Client& client)
	: ClientHandler(network_id, client) {
}

void GameClientHandler::on_create(Networkable& networkable, const Networkable& new_networkable) {
	std::vector<NetworkPacket> network_packets = ClientHandler::receive_incoming_create_network_packets();
	// TODO: Do something with it.
}
void GameClientHandler::on_update(Networkable& networkable) {
	Game& game = static_cast<Game&>(networkable);

	std::vector<NetworkPacket> network_packets = ClientHandler::receive_incoming_update_network_packets();

	for (auto& i_network_packet : network_packets) {
		std::string text;
		i_network_packet >> text;

		if (text == "start") {
			game.start();
			i_network_packet >> game.m_player_id;
		} else if (text == "ressources") {
			i_network_packet >> game.m_own_plasma_count;
			i_network_packet >> game.m_own_crystal_count;
		}
	}
}
void GameClientHandler::on_delete(Networkable& networkable) {
	std::vector<NetworkPacket> network_packets = ClientHandler::receive_incoming_delete_network_packets();
	// TODO: Do something with it.
}
