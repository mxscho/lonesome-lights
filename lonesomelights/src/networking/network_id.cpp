#include "networking/network_id.h"

NetworkID NetworkID::create_base() {
	return NetworkID(0, "base");
}

NetworkID::NetworkID(sf::Uint32 id, const std::string& description)
	: m_id(id),
	m_description(description) {
}
NetworkID::NetworkID()
	: NetworkID(0, "") {
}

bool NetworkID::operator==(const NetworkID& network_id) const {
	return m_id == network_id.m_id && m_description == network_id.m_description;
}

bool NetworkID::is_valid() const {
	return m_id != 0 && m_description != "";
}

sf::Uint32 NetworkID::get_id() const {
	return m_id;
}
void NetworkID::set_id(sf::Uint32 id) {
	m_id = id;
}
const std::string& NetworkID::get_description() const {
	return m_description;
}
void NetworkID::set_description(const std::string& description) {
	m_description = description;
}