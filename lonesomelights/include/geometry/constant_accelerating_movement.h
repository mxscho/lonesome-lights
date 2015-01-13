#ifndef __GEOMETRY__CONSTANT_ACCELERATING_MOVEMENT_H__
#define __GEOMETRY__CONSTANT_ACCELERATING_MOVEMENT_H__

#include "geometry/movement.h"

class ConstantAcceleratingMovement: public Movement {
public:
	ConstantAcceleratingMovement(float acceleration);
protected:
	float calculate_acceleration(float position, float velocity, float time) override;
private:
	float m_acceleration;
};

#endif