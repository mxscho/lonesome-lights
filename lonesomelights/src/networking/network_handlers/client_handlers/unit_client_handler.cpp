#include "networking/network_handlers/client_handlers/unit_client_handler.h"

// TEST
#include <iostream>
#include <string>

#include "networking/client.h"
#include "networking/network_packet.h"

UnitClientHandler::UnitClientHandler(const NetworkID& network_id, Client& client)
	: ClientHandler(network_id, client) {
}

void UnitClientHandler::on_create(const Networkable& networkable, const Networkable& new_networkable) {
	std::vector<NetworkPacket> network_packets = ClientHandler::receive_incoming_create_network_packets();
	// TODO: Do something with it.
}
void UnitClientHandler::on_update(const Networkable& networkable) {
	std::vector<NetworkPacket> network_packets = ClientHandler::receive_incoming_update_network_packets();
	// TODO: Do something with it.
	
	// TEST
	for (auto& i_network_packet : network_packets) {
		std::string text;
		if (!(i_network_packet >> text)) std::cout << "Client: ERROR" << std::endl;
		else std::cout << "Client: " << text << std::endl;
	}
}
void UnitClientHandler::on_delete(const Networkable& networkable) {
	std::vector<NetworkPacket> network_packets = ClientHandler::receive_incoming_delete_network_packets();
	// TODO: Do something with it.
}