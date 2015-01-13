#ifndef __GEOMETRY__MOVEMENT_H__
#define __GEOMETRY__MOVEMENT_H__

class Movement {
public:
	struct State {
		State(float displacement, float velocity);
		State();
		float displacement;
		float velocity;
	};
	
	State calculate(const State& initial_state, float initial_time, float delta_time);
protected:
	virtual float calculate_acceleration(float displacement, float velocity, float time) = 0;
private:
	struct Derivative {
		Derivative(float delta_position, float delta_velocity);
		Derivative();
		float delta_position;
		float delta_velocity;
	};
	
	Derivative evaluate_derivative(const State& initial_state, const Derivative& derivative, float initial_time, float delta_time);
};

#endif