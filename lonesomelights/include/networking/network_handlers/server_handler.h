#ifndef __NETWORKING__NETWORK_HANDLERS__SERVER_HANDLER_H__
#define __NETWORKING__NETWORK_HANDLERS__SERVER_HANDLER_H__

#include "networking/network_handler.h"

class NetworkID;
class Server;

class ServerHandler : public NetworkHandler {
public:
	ServerHandler(const NetworkID& network_id, Server& server);
protected:
	Server& m_server;
};

#endif