#include "networking/network_handlers/server_handlers/unit_server_handler.h"

// TEST
#include <iostream>
#include <string>

#include "networking/network_packet.h"
#include "networking/server.h"

UnitServerHandler::UnitServerHandler(const NetworkID& network_id, Server& server)
	: ServerHandler(network_id, server) {
}

void UnitServerHandler::on_create(const Networkable& networkable, const Networkable& new_networkable) {
	NetworkPacket network_packet = NetworkPacket::create_outgoing(m_network_id, NetworkPacket::Type::Create);
	// TODO: Fill network_packet with actual initial information.
	for (auto& i_participant : m_server.get_participants()) {
		i_participant.get().add_outgoing_network_packet(network_packet);
	}
}
void UnitServerHandler::on_update(const Networkable& networkable) {
	NetworkPacket network_packet = NetworkPacket::create_outgoing(m_network_id, NetworkPacket::Type::Update);
	// TODO: Fill network_packet with actual update information.
	
	// TEST
	static int wut = 0;
	if (wut >= 3) return;
	++wut;
	std::string text = "Servus!";
	network_packet << text;
	std::cout << "Server: " << text << std::endl;
	
	for (auto& i_participant : m_server.get_participants()) {
		i_participant.get().add_outgoing_network_packet(network_packet);
	}
}
void UnitServerHandler::on_delete(const Networkable& networkable) {
	NetworkPacket network_packet = NetworkPacket::create_outgoing(m_network_id, NetworkPacket::Type::Delete);
	for (auto& i_participant : m_server.get_participants()) {
		i_participant.get().add_outgoing_network_packet(network_packet);
	}
}