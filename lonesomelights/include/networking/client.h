#ifndef __NETWORKING__CLIENT_H__
#define __NETWORKING__CLIENT_H__

#include <memory>
#include <SFML/Network.hpp>

#include "networking/participant.h"

class Client {
public:
	Participant& get_participant() const;

	bool connect(const sf::IpAddress& address, unsigned short port);
	void disconnect();
	void update();

	bool is_connected() const;
	
	NetworkID create_base_network_id() const;
private:
	bool m_is_connected;
	sf::TcpSocket m_server_socket;
	std::unique_ptr<Participant> m_participant;
};

#endif
