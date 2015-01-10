#include "networking/network_handlers/client_handler.h"

#include "networking/client.h"
#include "networking/network_packet.h"
#include "networking/participant.h"

ClientHandler::ClientHandler(const NetworkID& network_id, Client& client)
	: NetworkHandler(network_id),
	m_client(client) {
}

std::vector<NetworkPacket> ClientHandler::receive_incoming_create_network_packets() {
	std::vector<NetworkPacket> incoming_create_network_packets;
	std::vector<NetworkPacket>& incoming_network_packets = m_client.get_participant().get_incoming_network_packets();
	for (auto i_network_packet = incoming_network_packets.begin(); i_network_packet != incoming_network_packets.end(); ) {
		if (i_network_packet->get_network_id() == m_network_id && i_network_packet->get_type() == NetworkPacket::Type::Create) {
			incoming_create_network_packets.push_back(*i_network_packet);
			i_network_packet = incoming_network_packets.erase(i_network_packet);
		} else {
			++i_network_packet;
		}
	}
	return incoming_create_network_packets;
}

std::vector<NetworkPacket> ClientHandler::receive_incoming_update_network_packets() {
	std::vector<NetworkPacket> incoming_update_network_packets;
	std::vector<NetworkPacket>& incoming_network_packets = m_client.get_participant().get_incoming_network_packets();
	for (auto i_network_packet = incoming_network_packets.begin(); i_network_packet != incoming_network_packets.end(); ) {
		if (i_network_packet->get_network_id() == m_network_id && i_network_packet->get_type() == NetworkPacket::Type::Update) {
			incoming_update_network_packets.push_back(*i_network_packet);
			i_network_packet = incoming_network_packets.erase(i_network_packet);
		} else {
			++i_network_packet;
		}
	}
	return incoming_update_network_packets;
}

std::vector<NetworkPacket> ClientHandler::receive_incoming_delete_network_packets() {
	std::vector<NetworkPacket> incoming_delete_network_packets;
	std::vector<NetworkPacket>& incoming_network_packets = m_client.get_participant().get_incoming_network_packets();
	for (auto i_network_packet = incoming_network_packets.begin(); i_network_packet != incoming_network_packets.end(); ) {
		if (i_network_packet->get_network_id() == m_network_id && i_network_packet->get_type() == NetworkPacket::Type::Delete) {
			incoming_delete_network_packets.push_back(*i_network_packet);
			i_network_packet = incoming_network_packets.erase(i_network_packet);
		} else {
			++i_network_packet;
		}
	}
	return incoming_delete_network_packets;
}