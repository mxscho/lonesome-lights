#ifndef __NETWORKING__NETWORK_ID_H__
#define __NETWORKING__NETWORK_ID_H__

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <SFML/Network.hpp>

class ServerNetwork;

class NetworkID {
public:
	static NetworkID create_base();

	NetworkID(sf::Uint32 id, const std::string& description);
	NetworkID();
	
	bool operator==(const NetworkID & network_id) const;
	
	bool is_valid() const;
	sf::Uint32 get_id() const;
	void set_id(sf::Uint32 id);
	const std::string& get_description() const;
	void set_description(const std::string& description);
private:
	sf::Uint32 m_id;
	std::string m_description;
};

#endif