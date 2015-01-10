#include "timer.h"

float Timer::get_current_time_seconds() const {
	return m_current_time.asSeconds();
}
float Timer::get_delta_time_seconds() const {
	return (m_current_time - m_last_time).asSeconds();
}
	
void Timer::advance() {
	m_last_time = m_current_time;
	m_current_time = m_clock.getElapsedTime();
}