#ifndef __GAME__MAP__DESTRUCTIBLE_ROCK_TILE_H__
#define __GAME__MAP__DESTRUCTIBLE_ROCK_TILE_H__

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game/map/tile.h"
#include "game/map/rock_tile.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

class Map;
class Timer;

class DestructibleRockTile : public RockTile {
public:
	DestructibleRockTile(const Map& map, unsigned int x, unsigned int y, const RockTile::CliffType& cliff_type);
	void draw(const Camera& camera) const override;
	
	bool is_selected() const;
	void unselect();
	void select(const glm::vec3& color);
	void unhover();
	void hover(const glm::vec3& color);
private:
	bool m_is_selected;
	bool m_is_hovered;
	glm::vec3 m_standard_color;
	glm::vec3 m_selection_color;
	glm::vec3 m_hover_color;
	
	
};

#endif
