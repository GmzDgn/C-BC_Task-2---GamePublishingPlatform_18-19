//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358

#pragma once
#include <windows.h>

typedef struct {
	LARGE_INTEGER start;
	LARGE_INTEGER stop;
} stopWatch;

class CStopWatch {
private:
	stopWatch timer;
	LARGE_INTEGER frequency;
	double LIToSecs(LARGE_INTEGER & L);
public:
	CStopWatch();
	// starts the stopwatch
	void startTimer();
	// stops the stopwatch
	void stopTimer();
	// returns a double from the difference of start and stop
	double getElapsedTime();
};