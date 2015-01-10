#ifndef __NETWORKING__PARTICIPANT_H__
#define __NETWORKING__PARTICIPANT_H__

#include <vector>
#include <SFML/Network.hpp>

#include "networking/network_packet.h"

class Client;
class Server;

class Participant {
friend class Server;
friend class Client;
public:
	struct ID {
		ID(const sf::IpAddress& address, unsigned short port);

		bool operator==(const ID& id) const;
	
		sf::IpAddress address;
		unsigned short port;
	};
	
	explicit Participant(const ID& id);

	bool operator==(const Participant& participant) const;
	
	const ID& get_id() const;
	std::vector<NetworkPacket>& get_incoming_network_packets();
	std::vector<NetworkPacket> receive_incoming_network_packets();
	void add_outgoing_network_packet(const NetworkPacket& network_packet);
private:
	void add_incoming_network_packet(const NetworkPacket& network_packet);
	std::vector<NetworkPacket> receive_outgoing_network_packets();

	ID m_id;	
	std::vector<NetworkPacket> m_incoming_network_packets;
	std::vector<NetworkPacket> m_outgoing_network_packets;
};

#endif