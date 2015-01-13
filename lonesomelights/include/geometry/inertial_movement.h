#include "geometry/movement.h"

class InertialMovement: public Movement {
public:
	InertialMovement(float start_position, float start_velocity, float end_position, float end_velocity, float max_velocity, float acceleration, float decceleration);
protected:
	float calculate_acceleration(float position, float velocity, float time) override;
private:
	float m_start_position;
	float m_start_velocity;
	float m_end_position;
	float m_end_velocity;
	float m_max_velocity;
	float m_acceleration;
	float m_decceleration;
};