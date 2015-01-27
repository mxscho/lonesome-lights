#ifndef __NETWORKING__NETWORKABLE_H__
#define __NETWORKING__NETWORKABLE_H__

class NetworkHandler;

class Networkable {
public:
	NetworkHandler* get_network_handler();
	void set_network_handler(NetworkHandler& network_handler);
	void unset_network_handler();
	
	void on_create(const Networkable& new_network_object);
	void on_update();
	void on_delete();
private:
	NetworkHandler* m_network_handler;
};

#endif
