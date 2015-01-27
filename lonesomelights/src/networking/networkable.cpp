#include "networking/networkable.h"

#include "networking/network_handler.h"

NetworkHandler* Networkable::get_network_handler() {
	return m_network_handler;
}
void Networkable::set_network_handler(NetworkHandler& network_handler) {
	m_network_handler = &network_handler;
}
void Networkable::unset_network_handler() {
	m_network_handler = nullptr;
}

void Networkable::on_create(const Networkable& new_networkable) {
	if (m_network_handler) {
		m_network_handler->on_create(*this, new_networkable);
	}
}
void Networkable::on_update() {
	if (m_network_handler) {
		m_network_handler->on_update(*this);
	}
}
void Networkable::on_delete() {
	if (m_network_handler) {
		m_network_handler->on_delete(*this);
	}
}
