#include "networking/participant.h"

Participant::ID::ID(const sf::IpAddress& address, unsigned short port):
	address(address),
	port(port) {
}

bool Participant::ID::operator==(const ID& id) const {
	return address == id.address && port == id.port;
}

Participant::Participant(const Participant::ID& id)
	: m_id(id) {
}

bool Participant::operator==(const Participant& participant) const {
	return m_id == participant.m_id;
}

const Participant::ID& Participant::get_id() const {
	return m_id;
}
std::vector<NetworkPacket>& Participant::get_incoming_network_packets() {
	return m_incoming_network_packets;
}
std::vector<NetworkPacket> Participant::receive_incoming_network_packets() {
	std::vector<NetworkPacket> incoming_network_packets = m_incoming_network_packets;
	m_incoming_network_packets.clear();
	return incoming_network_packets;
}
void Participant::add_outgoing_network_packet(const NetworkPacket& network_packet) {
	m_outgoing_network_packets.push_back(network_packet);
}

void Participant::add_incoming_network_packet(const NetworkPacket& network_packet) {
	m_incoming_network_packets.push_back(network_packet);
}
std::vector<NetworkPacket> Participant::receive_outgoing_network_packets() {
	std::vector<NetworkPacket> outgoing_network_packets = m_outgoing_network_packets;
	m_outgoing_network_packets.clear();
	return outgoing_network_packets;
}