#ifndef __GEOMETRY__CONSTANT_MOVEMENT_H__
#define __GEOMETRY__CONSTANT_MOVEMENT_H__

#include "geometry/movement.h"

class ConstantMovement: public Movement {
protected:
	float calculate_acceleration(float position, float velocity, float time) override;
};

#endif