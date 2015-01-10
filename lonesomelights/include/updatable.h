#ifndef __UPDATABLE_H__
#define __UPDATABLE_H__

class Timer;

class Updatable {
public:
	virtual void update(const Timer& timer) = 0;
};

#endif