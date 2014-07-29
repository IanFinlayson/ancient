// Timer.h
// Ian Finlayson
// Gives a nice interface to the PerformanceCounters

#ifndef TIMER_H
#define TIMER_H

#include <windows.h>

class Timer
{
private:
	LARGE_INTEGER frequency;	// Time between updates
	LARGE_INTEGER start_time;	// Initial time
	LARGE_INTEGER last_time;	// Time it was at last Update
	LARGE_INTEGER current_time;	// Time it is at this update

	unsigned int total_frames;	// Number of frames played

public:
	// Constructor
	Timer( )
	{
		QueryPerformanceFrequency( &frequency );
		QueryPerformanceCounter( &start_time );

		current_time = last_time = start_time;

		total_frames = 0;
	}

	// Updates the time...Should be called each frame
	void Update( )
	{
		last_time = current_time;
		QueryPerformanceCounter( &current_time );

		total_frames++;
	}

	// Returns the number of seconds between frames
	float GetElapsedTime( )
	{
		return ( ( (float)current_time.QuadPart - (float)last_time.QuadPart ) / (float)frequency.QuadPart );
	}

	// Returns the average FPS
	float GetFPS( )
	{
		float elapsed = ( ( (float)current_time.QuadPart - (float)start_time.QuadPart ) / (float)frequency.QuadPart );

		return (total_frames / elapsed);
	}
};

#endif