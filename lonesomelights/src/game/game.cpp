#include "game/game.h"

#include <glm/glm.hpp>

#include "game/units/laser_unit.h"

Game::Game()
	: Networkable(),
	Updatable(),
	m_map(Map::create_test_map(1.0F)), // TEST
	m_own_player(glm::vec3(0.0F, 0.0F, 0.8F)),
	m_opponent_player(glm::vec3(0.8F, 0.0F, 0.0F)),
	m_own_units(),
	m_opponent_units() {
	
	// TEST
	m_own_units.push_back(LaserUnit::create(glm::vec2(4.0F, 4.0F), m_map, m_own_player));
	m_own_units.push_back(LaserUnit::create(glm::vec2(2.0F, 2.0F), m_map, m_own_player));
	if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(m_own_units[0].get())) {
		own_laser_unit->start_shooting(glm::vec2(7.0F, 3.0F));
	}
	m_opponent_units.push_back(LaserUnit::create(glm::vec2(7.0F, 3.0F), m_map, m_opponent_player));
}

Map& Game::get_map() {
	return m_map;
}
std::vector<std::unique_ptr<Unit>>& Game::get_own_units() {
	return m_own_units;
}

void Game::draw(const Camera& camera) const {
	m_map.draw(camera);
	for (auto& i_own_unit : m_own_units) {
		if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			own_laser_unit->draw(camera);
		}
	}
	for (auto& i_opponent_unit : m_opponent_units) {
		if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			opponent_laser_unit->draw(camera);
		}
	}
}
void Game::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
	for (auto& i_own_unit : m_own_units) {
		if (LaserUnit* own_laser_unit = dynamic_cast<LaserUnit*>(i_own_unit.get())) {
			own_laser_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		}
	}
	for (auto& i_opponent_unit : m_opponent_units) {
		if (LaserUnit* opponent_laser_unit = dynamic_cast<LaserUnit*>(i_opponent_unit.get())) {
			opponent_laser_unit->draw_deferred(camera, color_texture, position_texture, normal_texture, depth_texture);
		}
	}
}

void Game::update(const Timer& timer) {
	for (auto& i_own_unit : m_own_units) {
		i_own_unit->update(timer);
	}
	for (auto& i_opponent_unit : m_opponent_units) {
		i_opponent_unit->update(timer);
	}
}