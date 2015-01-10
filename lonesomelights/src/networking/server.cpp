#include "networking/server.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include "networking/network_packet.h"

const std::vector<std::reference_wrapper<Participant>>& Server::get_participants() {
	return m_participant_references;
}

bool Server::start(unsigned int port) {
	stop();

	std::cout << "Server: Initializing listening at port " << port << "..." << std::endl;
	if (m_listener_socket.listen(port) == sf::Socket::Status::Error) {
		std::cerr << "Server: Binding server to listen at specified port failed." << std::endl;
		stop();
		return false;
	}
	std::cout << "Server: Initialization has been successful." << std::endl;
	m_listener_socket.setBlocking(false);
	m_is_listening = true;
	return true;
}
void Server::stop() {
	m_listener_socket.close();
	for (auto i_client_socket = m_client_sockets.begin(); i_client_socket != m_client_sockets.end(); ++i_client_socket) {
		(*i_client_socket)->disconnect();
	}
	m_client_sockets.clear();
	m_participants.clear();
	m_participant_references.clear();
	m_is_listening = false;
}
void Server::update() {
	if (!m_is_listening) {
		return;
	}

	// Accept pending connection.
	
	std::unique_ptr<sf::TcpSocket> client_socket(new sf::TcpSocket());
	sf::Socket::Status status = m_listener_socket.accept(*client_socket);
	if (status == sf::Socket::Status::Error) {
		std::cerr << "Server: An unexpected error occured during listening." << std::endl;
		stop();
		return;
	} else if (status != sf::Socket::Status::NotReady) {
		Participant participant(Participant::ID(client_socket->getRemoteAddress(), client_socket->getRemotePort()));
		if (std::find(m_participants.begin(), m_participants.end(), participant) != m_participants.end()) {
			std::cerr << "Server: Could not accept connection because an equal participant already exists." << std::endl;	
			return;
		}
		client_socket->setBlocking(false);
		m_client_sockets.push_back(std::move(client_socket));
		m_participants.push_back(participant);
		m_participant_references.push_back(std::ref(m_participants.back()));
		std::cerr << "Server: Accepted new connection." << std::endl;
	}
	
	auto i_client_socket = m_client_sockets.begin();
	auto i_participant = m_participants.begin();
	auto i_participant_reference = m_participant_references.begin();
	while(i_client_socket != m_client_sockets.end() && i_participant != m_participants.end() && i_participant_reference != m_participant_references.end()) {
		// Send network packets to clients.
		
		sf::Socket::Status send_status = sf::Socket::Status::Done;
		for (auto i_network_packet : i_participant->receive_outgoing_network_packets()) {
			do {
				send_status = (*i_client_socket)->send(i_network_packet);
			} while (send_status == sf::Socket::Status::NotReady);
			if (send_status != sf::Socket::Status::Done) {
				break;
			}
		}
		
		// Receive network packets from clients.
		
		sf::Packet packet;
		sf::Socket::Status receive_status = (*i_client_socket)->receive(packet);
		while (send_status == sf::Socket::Status::Done && receive_status == sf::Socket::Status::Done) {
			i_participant->add_incoming_network_packet(NetworkPacket::create_incoming(packet));
			receive_status = (*i_client_socket)->receive(packet);
		}
		
		if (send_status == sf::Socket::Status::Disconnected || receive_status == sf::Socket::Status::Disconnected) {
			std::cerr << "Server: The client at " << (*i_client_socket)->getRemoteAddress() << ":" << (*i_client_socket)->getRemotePort() << " disconnected." << std::endl;
			(*i_client_socket)->disconnect();
			i_client_socket = m_client_sockets.erase(i_client_socket);
			i_participant = m_participants.erase(i_participant);
			i_participant_reference = m_participant_references.erase(i_participant_reference);
		} else {
			if (send_status == sf::Socket::Status::Error) {
				std::cerr << "Server: An unexpected error occured while sending a network packet." << std::endl;
				stop();
				return;
			}
			if (receive_status == sf::Socket::Status::Error) {
				std::cerr << "Server: An unexpected error occured while receiving a network packet." << std::endl;
				stop();
				return;
			}
			++i_client_socket;
			++i_participant;
		}
	}
}

NetworkID Server::create_base_network_id() {
	m_current_network_id = NetworkID::create_base();
	return m_current_network_id;
}
NetworkID Server::create_next_network_id(const std::string& description) {
	m_current_network_id.set_id(m_current_network_id.get_id() + 1);
	m_current_network_id.set_description(description);
	return m_current_network_id;
}