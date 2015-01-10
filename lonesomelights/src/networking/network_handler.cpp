#include "networking/network_handler.h"

NetworkHandler::NetworkHandler(const NetworkID& network_id)
	: m_network_id(network_id) {
}

void NetworkHandler::on_create(const Networkable& networkable, const Networkable& new_networkable) {
}
void NetworkHandler::on_update(const Networkable& networkable) {
}
void NetworkHandler::on_delete(const Networkable& networkable) {
}