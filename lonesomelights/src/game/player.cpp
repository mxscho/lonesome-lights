#include "game/player.h"

Player::Player(const glm::vec3& color)
	: m_color(color) {
}
	
const glm::vec3& Player::get_color() const {
	return m_color;
}