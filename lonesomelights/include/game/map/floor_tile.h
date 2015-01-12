#ifndef __GAME__MAP__FLOOR_TILE_H__
#define __GAME__MAP__FLOOR_TILE_H__

#include "game/map/tile.h"

class Camera;
class RenderProgram;

class FloorTile : public Tile {
public:
	void draw(const RenderProgram& render_program, const Camera& camera) const override final;
};

#endif