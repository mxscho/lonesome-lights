#ifndef __GAME__UNIT_H__
#define __GAME__UNIT_H__

#include "geometry/transformable.h"
#include "networking/networkable.h"
#include "rendering/drawable.h"
#include "updatable.h"

class Camera;
class RenderProgram;
class Timer;

class Unit : public Drawable, public Networkable, public Updatable, public Transformable {
public:
	void draw(const RenderProgram& render_program, const Camera& camera) const override final;
	
	void update(const Timer& timer) override final;
};

#endif