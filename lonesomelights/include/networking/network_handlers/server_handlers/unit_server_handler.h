#ifndef __NETWORKING__NETWORK_HANDLERS__SERVER_HANDLERS__UNIT_SERVER_HANDLER_H__
#define __NETWORKING__NETWORK_HANDLERS__SERVER_HANDLERS__UNIT_SERVER_HANDLER_H__

#include "networking/network_handlers/server_handler.h"

class NetworkID;
class Networkable;
class Server;

class UnitServerHandler: public ServerHandler {
public:
	UnitServerHandler(const NetworkID& network_id, Server& server);
	
	void on_create(const Networkable& networkable, const Networkable& new_networkable) override final;
	void on_update(const Networkable& networkable) override final;
	void on_delete(const Networkable& networkable) override final;
};

#endif