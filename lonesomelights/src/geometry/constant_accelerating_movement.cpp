#include "geometry/constant_accelerating_movement.h"

ConstantAcceleratingMovement::ConstantAcceleratingMovement(float acceleration)
	: Movement(),
	m_acceleration(acceleration) {
}
float ConstantAcceleratingMovement::calculate_acceleration(float displacement, float velocity, float time) {
	return m_acceleration;
}