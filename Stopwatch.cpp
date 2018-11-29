//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358
#include <windows.h>

#ifndef hr_stopwatch
#include "Stopwatch.h"
#define hr_stopwatch
#endif

double CStopWatch::LIToSecs(LARGE_INTEGER & L) {
	return ((double)L.QuadPart / (double)frequency.QuadPart);
}

CStopWatch::CStopWatch() {
	timer.start.QuadPart = 0;
	timer.stop.QuadPart = 0;
	QueryPerformanceFrequency(&frequency);
}

void CStopWatch::startTimer() {
	QueryPerformanceCounter(&timer.start);
}

void CStopWatch::stopTimer() {
	QueryPerformanceCounter(&timer.stop);
}

double CStopWatch::getElapsedTime() {
	LARGE_INTEGER time;
	time.QuadPart = timer.stop.QuadPart - timer.start.QuadPart;
	return LIToSecs(time);
}
