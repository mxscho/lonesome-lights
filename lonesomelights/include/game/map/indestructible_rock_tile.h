#ifndef __GAME__MAP__INDESTRUCTIBLE_ROCK_TILE_H__
#define __GAME__MAP__INDESTRUCTIBLE_ROCK_TILE_H__

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game/map/tile.h"
#include "game/map/rock_tile.h"
#include "rendering/opengl/vertex_array_object.h"
#include "rendering/opengl/vertex_buffer_object.h"

class Map;
class Timer;

class IndestructibleRockTile : public RockTile {
public:
	IndestructibleRockTile(const Map& map, unsigned int x, unsigned int y, const RockTile::CliffType& cliff_type);
	void draw(const Camera& camera) const override;
	
private:
	glm::vec3 m_standard_color;	
};

#endif
