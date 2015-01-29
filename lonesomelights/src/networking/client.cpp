#include "networking/client.h"

#include <iostream>

#include "networking/network_packet.h"

Participant& Client::get_participant() const {
	return *m_participant;
}

bool Client::is_connected() const {
	return m_is_connected;
}

bool Client::connect(const sf::IpAddress& address, unsigned short port) {
	disconnect();

	std::cout << "Client: Connecting to server at " << address << ":" << port << "..." << std::endl;
	if (m_server_socket.connect(address, port) == sf::Socket::Error) {
		std::cerr << "Client: Attempt to connect to server failed." << std::endl;
		disconnect();
		return false;
	}
	std::cout << "Client: Sucessfully connected to the server." << std::endl;
	m_server_socket.setBlocking(false);
	m_participant.reset(new Participant(Participant::ID(address, port)));
	m_is_connected = true;
	return true;
}
void Client::disconnect() {
	m_server_socket.disconnect();
	m_participant.release();
	m_is_connected = false;
}
void Client::update() {
	if (!m_is_connected) {
		return;
	}
	
	// Send network packets to server.
		
	sf::Socket::Status send_status = sf::Socket::Status::Done;
	for (auto i_network_packet : m_participant->receive_outgoing_network_packets()) {
		do {
			send_status = m_server_socket.send(i_network_packet);
		} while (send_status == sf::Socket::Status::NotReady);
		if (send_status != sf::Socket::Status::Done) {
			break;
		}
	}
	
	// Receive network packets from server.

	sf::Packet packet;
	sf::Socket::Status receive_status = m_server_socket.receive(packet);
	while (send_status == sf::Socket::Status::Done && receive_status == sf::Socket::Status::Done) {
		m_participant->add_incoming_network_packet(NetworkPacket::create_incoming(packet));
		receive_status = m_server_socket.receive(packet);
	}
	
	if (send_status == sf::Socket::Status::Disconnected || receive_status == sf::Socket::Status::Disconnected) {
		std::cerr << "Server: The client at " << m_server_socket.getRemoteAddress() << ":" << m_server_socket.getRemotePort() << " disconnected." << std::endl;
		disconnect();
		return;
	} else {
		if (send_status == sf::Socket::Status::Error) {
			std::cerr << "Server: An unexpected error occured while sending a network packet." << std::endl;
			disconnect();
			return;
		}
		if (receive_status == sf::Socket::Status::Error) {
			std::cerr << "Server: An unexpected error occured while receiving a network packet." << std::endl;
			disconnect();
			return;
		}
	}
}

NetworkID Client::create_base_network_id() const {
	return NetworkID::create_base();
}
