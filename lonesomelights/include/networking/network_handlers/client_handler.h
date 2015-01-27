#ifndef __NETWORKING__NETWORK_HANDLERS__CLIENT_HANDLER_H__
#define __NETWORKING__NETWORK_HANDLERS__CLIENT_HANDLER_H__

#include "networking/network_handler.h"

class Client;
class NetworkID;
class NetworkPacket;

class ClientHandler : public NetworkHandler {
public:
	ClientHandler(const NetworkID& network_id, Client& client);

	std::vector<NetworkPacket> receive_incoming_create_network_packets();
	std::vector<NetworkPacket> receive_incoming_update_network_packets();
	std::vector<NetworkPacket> receive_incoming_delete_network_packets();
protected:
	Client& m_client;
};

#endif
