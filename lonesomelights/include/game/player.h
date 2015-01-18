#ifndef __GAME__PLAYER_H__
#define __GAME__PLAYER_H__

#include <glm/glm.hpp>

class Player {
public:
	Player(const glm::vec3& color);
	
	const glm::vec3& get_color() const;
private:
	glm::vec3 m_color;
};

#endif