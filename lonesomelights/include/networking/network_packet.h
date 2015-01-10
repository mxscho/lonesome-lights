#ifndef __NETWORKING__NETWORK_PACKET_H__
#define __NETWORKING__NETWORK_PACKET_H__

#include <string>
#include <SFML/Network.hpp>

#include "networking/network_id.h"

class NetworkPacket: public sf::Packet {
public:
	enum class Type : sf::Int8 {
		Create = 0,
		Update = 1,
		Delete = 2
	};
	
	static NetworkPacket create_incoming(sf::Packet& packet);
	static NetworkPacket create_outgoing(const NetworkID& network_id, const Type& type);
	
	static const sf::Int8 c_magic_number;
	
	const NetworkID& get_network_id() const;
	const Type& get_type() const;

	NetworkPacket& operator>>(bool& data);
    NetworkPacket& operator>>(sf::Int8& data);
    NetworkPacket& operator>>(sf::Uint8& data);
    NetworkPacket& operator>>(sf::Int16& data);
    NetworkPacket& operator>>(sf::Uint16& data);
    NetworkPacket& operator>>(sf::Int32& data);
    NetworkPacket& operator>>(sf::Uint32& data);
    NetworkPacket& operator>>(float& data);
    NetworkPacket& operator>>(double& data);
    NetworkPacket& operator>>(char* data);
    NetworkPacket& operator>>(std::string& data);
    NetworkPacket& operator>>(wchar_t* data);
    NetworkPacket& operator>>(std::wstring& data);
    NetworkPacket& operator>>(sf::String& data);

    NetworkPacket& operator<<(bool data);
    NetworkPacket& operator<<(sf::Int8 data);
    NetworkPacket& operator<<(sf::Uint8 data);
    NetworkPacket& operator<<(sf::Int16 data);
    NetworkPacket& operator<<(sf::Uint16 data);
    NetworkPacket& operator<<(sf::Int32 data);
    NetworkPacket& operator<<(sf::Uint32 data);
    NetworkPacket& operator<<(float data);
    NetworkPacket& operator<<(double data);
    NetworkPacket& operator<<(const char* data);
    NetworkPacket& operator<<(const std::string& data);
    NetworkPacket& operator<<(const wchar_t* data);
    NetworkPacket& operator<<(const std::wstring& data);
    NetworkPacket& operator<<(const sf::String& data);
private:
	NetworkPacket(NetworkID network_id, Type type);
	
	NetworkID m_network_id;
	Type m_type;
};

#endif