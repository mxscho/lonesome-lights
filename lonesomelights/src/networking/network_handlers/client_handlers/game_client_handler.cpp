#include "networking/network_handlers/client_handlers/game_client_handler.h"

#include <string>

#include "game/client_game.h"

#include "networking/client.h"
#include "networking/network_packet.h"

GameClientHandler::GameClientHandler(const NetworkID& network_id, Client& client)
	: ClientHandler(network_id, client) {
}

void GameClientHandler::on_create(Networkable& networkable, const Networkable& new_networkable) {
	std::vector<NetworkPacket> network_packets = ClientHandler::receive_incoming_create_network_packets();
}
void GameClientHandler::on_update(Networkable& networkable) {
	ClientGame& game = static_cast<ClientGame&>(networkable);

	std::vector<NetworkPacket> network_packets;

	std::vector<NetworkPacket>& incoming_network_packets = m_client.get_participant().get_incoming_network_packets();
	for (auto i_network_packet = incoming_network_packets.begin(); i_network_packet != incoming_network_packets.end(); ++i_network_packet) {
		if (i_network_packet->get_network_id() == m_network_id && i_network_packet->get_type() == NetworkPacket::Type::Update) {
			network_packets.push_back(*i_network_packet);
		}
	}

	//std::vector<NetworkPacket> network_packets = ClientHandler::receive_incoming_update_network_packets();

	for (auto& i_network_packet : network_packets) {
		std::string text;
		i_network_packet >> text;

		if (text == "start") {
			game.start();
			i_network_packet >> game.m_player_id;
		}
	}
}
void GameClientHandler::on_delete(Networkable& networkable) {
	std::vector<NetworkPacket> network_packets = ClientHandler::receive_incoming_delete_network_packets();
}
