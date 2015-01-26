#ifndef __NETWORKING__NETWORK_HANDLERS__CLIENT_HANDLERS__GAME_CLIENT_HANDLER_H__
#define __NETWORKING__NETWORK_HANDLERS__CLIENT_HANDLERS__GAME_CLIENT_HANDLER_H__

#include "networking/network_handlers/client_handler.h"

class Client;
class NetworkID;
class Networkable;

class GameClientHandler: public ClientHandler {
public:
	GameClientHandler(const NetworkID& network_id, Client& client);
	
	void on_create(Networkable& networkable, const Networkable& new_networkable) override final;
	void on_update(Networkable& networkable) override final;
	void on_delete(Networkable& networkable) override final;
private:
	struct Data {
		
	};
};

#endif
