#include "game/map/destructible_rock_tile.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "game/map/map.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"


DestructibleRockTile::DestructibleRockTile(const Map& map, unsigned int x, unsigned int y, const CliffType& cliff_type)
	: Attackable(glm::translate(glm::vec3(0.5F, 1.0F, 0.5F)), *this, 100.0F, 100.0F),
	RockTile(RockTile::create(map, x, y, cliff_type)),
	m_is_selected(false),
	m_is_hovered(false),
	m_standard_color(0.4f, 0.4f, 0.4f),
	m_selection_color(0.7f, 0.7f, 0.7f),
	m_hover_color(0.9f, 0.9f, 0.9f) {
	
	RockTile::set_color(m_standard_color);
}

glm::vec2 DestructibleRockTile::get_position_vec2() const {
	return glm::vec2(get_x() + 0.5F * get_size(), get_y() + 0.5F * get_size());
}

void DestructibleRockTile::draw(const Camera& camera) const {
	RockTile::draw(camera);
}

void DestructibleRockTile::update(const Timer& timer) {
	Attackable::update(timer);
}

bool DestructibleRockTile::is_selected() const {
	return m_is_selected;
}

void DestructibleRockTile::unselect() {
	m_is_selected = false;
	if (m_is_hovered) {
		set_color(m_hover_color);
	} else {
		set_color(m_standard_color);
	}
}

void DestructibleRockTile::select(const glm::vec3& color) {
	m_is_selected = true;
	m_selection_color = color;
	if (m_is_hovered) {
		set_color(m_hover_color);
	} else {
		set_color(m_selection_color);
	}
}

void DestructibleRockTile::unhover() {
	if (m_is_selected) {
		set_color(m_selection_color);
	} else {
		set_color(m_standard_color);
	}
	m_is_hovered = false;
}

void DestructibleRockTile::hover(const glm::vec3& color) {
	m_is_hovered = true;
	m_hover_color = color;
	set_color(m_hover_color);
}
