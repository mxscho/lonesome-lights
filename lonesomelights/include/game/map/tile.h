#ifndef __GAME__MAP__TILE_H__
#define __GAME__MAP__TILE_H__

#include "geometry/transformable.h"
#include "networking/networkable.h"
#include "rendering/drawable.h"

class RenderProgram;
class Timer;

class Tile : public Drawable, public Networkable, public Transformable {
};

#endif