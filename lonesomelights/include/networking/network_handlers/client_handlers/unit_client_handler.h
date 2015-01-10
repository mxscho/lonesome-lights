#ifndef __NETWORKING__NETWORK_HANDLERS__CLIENT_HANDLERS__UNIT_CLIENT_HANDLER_H__
#define __NETWORKING__NETWORK_HANDLERS__CLIENT_HANDLERS__UNIT_CLIENT_HANDLER_H__

#include "networking/network_handlers/client_handler.h"

class Client;
class NetworkID;
class Networkable;

class UnitClientHandler: public ClientHandler {
public:
	UnitClientHandler(const NetworkID& network_id, Client& client);
	
	void on_create(const Networkable& networkable, const Networkable& new_networkable) override final;
	void on_update(const Networkable& networkable) override final;
	void on_delete(const Networkable& networkable) override final;
};

#endif