#include "networking/network_packet.h"

NetworkPacket NetworkPacket::create_incoming(sf::Packet& packet) {
	sf::Int8 magic_number;
	sf::Uint32 network_id_id;
	std::string network_id_description;
	sf::Int8 type;
	if (!(packet >> magic_number) || magic_number != NetworkPacket::c_magic_number ||
		!(packet >> network_id_id) || !(packet >> network_id_description) ||
		!(packet >> type)) {
		throw; // TODO: Exception handling.
	}
	
	NetworkID network_id(network_id_id, network_id_description);
	NetworkPacket network_packet(network_id, static_cast<NetworkPacket::Type>(type));
	network_packet.append(packet.getData(), packet.getDataSize());
	
	network_packet >> magic_number;
	network_packet >> network_id_id;
	network_packet >> network_id_description;
	network_packet >> type;
	
	return network_packet;
}

NetworkPacket NetworkPacket::create_outgoing(const NetworkID& network_id, const NetworkPacket::Type& type) {
	NetworkPacket network_packet(network_id, type);
	network_packet << NetworkPacket::c_magic_number;
	network_packet << network_id.get_id();
	network_packet << network_id.get_description();
	network_packet << static_cast<sf::Int8>(type);
	return network_packet;
}

const sf::Int8 NetworkPacket::c_magic_number = 42;

const NetworkID& NetworkPacket::get_network_id() const {
	return m_network_id;
}
const NetworkPacket::Type& NetworkPacket::get_type() const {
	return m_type;
}

NetworkPacket& NetworkPacket::operator>>(bool& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(sf::Int8& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(sf::Uint8& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(sf::Int16& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(sf::Uint16& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(sf::Int32& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(sf::Uint32& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(float& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(double& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(char* data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(std::string& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(wchar_t* data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(std::wstring& data) {
	sf::Packet::operator>>(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator>>(sf::String& data) {
	sf::Packet::operator>>(data);
	return *this;
}

NetworkPacket& NetworkPacket::operator<<(bool data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(sf::Int8 data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(sf::Uint8 data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(sf::Int16 data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(sf::Uint16 data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(sf::Int32 data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(sf::Uint32 data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(float data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(double data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(const char* data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(const std::string& data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(const wchar_t* data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(const std::wstring& data) {
	sf::Packet::operator<<(data);
	return *this;
}
NetworkPacket& NetworkPacket::operator<<(const sf::String& data) {
	sf::Packet::operator<<(data);
	return *this;
}

NetworkPacket::NetworkPacket(NetworkID network_id, Type type)
	: sf::Packet(),
	m_network_id(network_id),
	m_type(type) {
}