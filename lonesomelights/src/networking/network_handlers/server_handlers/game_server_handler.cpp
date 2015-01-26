#include "networking/network_handlers/server_handlers/game_server_handler.h"

#include <string>

#include "game/game.h"

#include "networking/network_packet.h"
#include "networking/server.h"

GameServerHandler::GameServerHandler(const NetworkID& network_id, Server& server)
	: ServerHandler(network_id, server) {
}

void GameServerHandler::on_create(Networkable& networkable, const Networkable& new_networkable) {
	NetworkPacket network_packet = NetworkPacket::create_outgoing(m_network_id, NetworkPacket::Type::Create);
	// TODO: Fill network_packet with actual initial information.
	for (auto& i_participant : m_server.get_participants()) {
		i_participant.get().add_outgoing_network_packet(network_packet);
	}
}
void GameServerHandler::on_update(Networkable& networkable) {
	NetworkPacket network_packet = NetworkPacket::create_outgoing(m_network_id, NetworkPacket::Type::Update);
	Game& game = static_cast<Game&>(networkable);

	if (!game.has_started()) {
		game.start();

		std::string text = "start";
		network_packet << text;
	}

	for (auto& i_participant : m_server.get_participants()) {
		i_participant.get().add_outgoing_network_packet(network_packet);
	}
}
void GameServerHandler::on_delete(Networkable& networkable) {
	NetworkPacket network_packet = NetworkPacket::create_outgoing(m_network_id, NetworkPacket::Type::Delete);
	for (auto& i_participant : m_server.get_participants()) {
		i_participant.get().add_outgoing_network_packet(network_packet);
	}
}
