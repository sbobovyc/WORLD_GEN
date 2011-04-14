#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;

class Timer 
{
public:	
	Timer(string description);
	friend std::ostream& operator<< (std::ostream& o, Timer const& timer);
	void start();
	void stop();

private:
	string description;
	struct timespec start_time, end_time;
};

#endif
