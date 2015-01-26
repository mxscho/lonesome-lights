#ifndef __TIMER_H__
#define __TIMER_H__

#include <SFML/System.hpp>

class Timer {
public:
	float get_current_time_seconds() const;
	float get_delta_time_seconds() const;
	
	void advance();
	void reset(float start_time_seconds);
private:
	sf::Clock m_clock;
	sf::Time m_current_time;
	sf::Time m_last_time;

	float m_start_time_seconds;
};

#endif
