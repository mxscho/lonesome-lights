#ifndef __NETWORKING__SERVER_H__
#define __NETWORKING__SERVER_H__

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Network.hpp>

#include "networking/network_id.h"
#include "networking/participant.h"

class Server {
public:
	const std::vector<std::reference_wrapper<Participant>>& get_participants();
	bool start(unsigned int port);
	void stop();
	void update();
	
	NetworkID create_base_network_id();
	NetworkID create_next_network_id(const std::string& description);
private:
	bool m_is_listening;
	sf::TcpListener m_listener_socket;
	std::vector<std::unique_ptr<sf::TcpSocket>> m_client_sockets;
	std::vector<Participant> m_participants;
	std::vector<std::reference_wrapper<Participant>> m_participant_references;
	NetworkID m_current_network_id;
};

#endif
