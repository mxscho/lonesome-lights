#ifndef __NETWORKING__NETWORK_HANDLER_H__
#define __NETWORKING__NETWORK_HANDLER_H__

#include <SFML/Network.hpp>

#include "networking/network_id.h"

class Networkable;

class NetworkHandler {
public:
	NetworkHandler(const NetworkID& network_id);

	virtual void on_create(const Networkable& networkable, const Networkable& new_networkable) = 0;
	virtual void on_update(const Networkable& networkable) = 0;
	virtual void on_delete(const Networkable& networkable) = 0;
protected:
	NetworkID m_network_id;
};

#endif