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

Timer::Timer(string timer_description)
{
	description = timer_description;
}

void Timer::start()
{
	if(clock_gettime(CLOCK_MONOTONIC, &start_time) == -1)
		perror("Failed to get time");
}

void Timer::stop()
{
	if(clock_gettime(CLOCK_MONOTONIC, &end_time) == -1)
		perror("Failed to get time");
}

std::ostream& operator<< (std::ostream& o, Timer const& timer)
{
	timespec temp;
	if ((timer.end_time.tv_nsec-timer.start_time.tv_nsec)<0) {
		temp.tv_sec = timer.end_time.tv_sec-timer.start_time.tv_sec-1;
		temp.tv_nsec = 1000000000+timer.end_time.tv_nsec-timer.start_time.tv_nsec;
	} else {
		temp.tv_sec = timer.end_time.tv_sec-timer.start_time.tv_sec;
		temp.tv_nsec = timer.end_time.tv_nsec-timer.start_time.tv_nsec;
	}

	return o << "Runtime of " << timer.description << " is " << temp.tv_sec  << " seconds, " << temp.tv_nsec << " nanoseconds." << std::endl;
}

