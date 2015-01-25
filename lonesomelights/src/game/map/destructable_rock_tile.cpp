#include "game/map/destructable_rock_tile.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "game/map/map.h"
#include "rendering/opengl/render_program.h"
#include "rendering/opengl/render_programs.h"
#include "rendering/opengl/textures.h"


DestructableRockTile::DestructableRockTile(const Map& map, unsigned int x, unsigned int y, const CliffType& cliff_type)
	: RockTile(RockTile::create(map, x, y, cliff_type)),
	m_is_selected(false),
	m_is_hovered(false),
	m_standard_color(0.4f, 0.4f, 0.4f),
	m_selection_color(0.7f, 0.7f, 0.7f),
	m_hover_color(0.9f, 0.9f, 0.9f) {
	
	RockTile::set_color(m_standard_color);
}

void DestructableRockTile::draw(const Camera& camera) const {
	RockTile::draw(camera);
}

bool DestructableRockTile::is_selected() const {
	return m_is_selected;
}

void DestructableRockTile::unselect() {
	m_is_selected = false;
	if (m_is_hovered) {
		set_color(m_hover_color);
	} else {
		set_color(m_standard_color);
	}
}

void DestructableRockTile::select(const glm::vec3& color) {
	m_is_selected = true;
	m_selection_color = color;
	if (m_is_hovered) {
		set_color(m_hover_color);
	} else {
		set_color(m_selection_color);
	}
}

void DestructableRockTile::unhover() {
	if (m_is_selected) {
		set_color(m_selection_color);
	} else {
		set_color(m_standard_color);
	}
	m_is_hovered = false;
}

void DestructableRockTile::hover(const glm::vec3& color) {
	if (m_is_selected || m_is_hovered) {
		return;
	}
	m_is_hovered = true;
	m_hover_color = color;
	set_color(m_hover_color);
}
