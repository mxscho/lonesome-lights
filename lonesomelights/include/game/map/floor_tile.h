#ifndef __GAME__MAP__FLOOR_TILE_H__
#define __GAME__MAP__FLOOR_TILE_H__

#include "game/map/tile.h"

class RenderProgram;

class FloorTile : public Tile {
public:
	void draw(const RenderProgram& render_program) const override final;
};

#endif