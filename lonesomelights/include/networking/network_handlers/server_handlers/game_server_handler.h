#ifndef __NETWORKING__NETWORK_HANDLERS__SERVER_HANDLERS__GAME_SERVER_HANDLER_H__
#define __NETWORKING__NETWORK_HANDLERS__SERVER_HANDLERS__GAME_SERVER_HANDLER_H__

#include "networking/network_handlers/server_handler.h"

class NetworkID;
class Networkable;
class Server;

class GameServerHandler: public ServerHandler {
public:
	GameServerHandler(const NetworkID& network_id, Server& server);
	
	void on_create(Networkable& networkable, const Networkable& new_networkable) override final;
	void on_update(Networkable& networkable) override final;
	void on_delete(Networkable& networkable) override final;
};

#endif
