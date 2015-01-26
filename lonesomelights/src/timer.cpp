#include "timer.h"

float Timer::get_current_time_seconds() const {
	return m_start_time_seconds + m_current_time.asSeconds();
}
float Timer::get_delta_time_seconds() const {
	return (m_current_time - m_last_time).asSeconds();
}
	
void Timer::advance() {
	m_last_time = m_current_time;
	m_current_time = m_clock.getElapsedTime();
}

void Timer::reset(float start_time_seconds) {
	m_clock.restart();
	m_start_time_seconds = start_time_seconds;
}
