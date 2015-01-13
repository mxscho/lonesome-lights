#include "geometry/movement.h"

#include <array>

Movement::State::State(float displacement, float velocity)
	: displacement(displacement),
	velocity(velocity) {
}
Movement::State::State()
	: State(0.0F, 0.0F) {
}

Movement::State Movement::calculate(const Movement::State& initial_state, float initial_time, float delta_time) {
	// Update movement using integration and the iterative Runge–Kutta method with step-size 4.

	std::array<Movement::Derivative, 4> derivatives;
	derivatives[0] = evaluate_derivative(initial_state, Movement::Derivative(), initial_time, 0.0F * delta_time);
	derivatives[1] = evaluate_derivative(initial_state, derivatives[0], initial_time, 0.5F * delta_time);
	derivatives[2] = evaluate_derivative(initial_state, derivatives[1], initial_time, 0.5F * delta_time);
	derivatives[3] = evaluate_derivative(initial_state, derivatives[2], initial_time, 1.0F * delta_time);
	
	float delta_position_delta_time = 1.0F / 6.0F * (
		1.0F * derivatives[0].delta_position +
		2.0F * derivatives[1].delta_position +
		2.0F * derivatives[2].delta_position +
		1.0F * derivatives[3].delta_position
	);
    float delta_velocity_delta_time = 1.0F / 6.0F * (
		1.0F * derivatives[0].delta_velocity +
		2.0F * derivatives[1].delta_velocity +
		2.0F * derivatives[2].delta_velocity +
		1.0F * derivatives[3].delta_velocity
	);

	Movement::State state(
		initial_state.displacement + delta_time * delta_position_delta_time,
		initial_state.velocity + delta_time * delta_velocity_delta_time
	);
	return state;
}

Movement::Derivative::Derivative(float delta_position, float delta_velocity)
	: delta_position(delta_position),
	delta_velocity(delta_velocity) {
}
Movement::Derivative::Derivative()
	: Derivative(0.0F, 0.0F) {
}

Movement::Derivative Movement::evaluate_derivative(const Movement::State& initial_state, const Movement::Derivative& input_derivative, float initial_time, float delta_time) {
	Movement::State new_state;
    new_state.displacement = initial_state.displacement + delta_time * input_derivative.delta_position;
	new_state.velocity = initial_state.velocity + delta_time * input_derivative.delta_velocity;

    Movement::Derivative output_derivative;
    output_derivative.delta_position = new_state.velocity;
    output_derivative.delta_velocity = calculate_acceleration(new_state.displacement, new_state.velocity, initial_time + delta_time);
	
    return output_derivative;
}