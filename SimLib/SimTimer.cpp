/********************************************************************
*	SimTimer.cpp				 Ian Finlayson 2005					*
*																	*
********************************************************************/

#include <SDL/SDL.h>

#include "SimTimer.h"

Sim::Timer::Timer( )
{
	total_frames = 0;
	
	start_time = current_time = next_last_time = SDL_GetTicks( );

	paused = false;
}

void Sim::Timer::Update( )
{
	total_frames++;

	next_last_time = current_time;

	current_time = SDL_GetTicks( );
}

unsigned int Sim::Timer::GetFPS( )
{
	unsigned int total_seconds = (current_time - start_time) / 1000;

	return total_frames / total_seconds;
}

unsigned int Sim::Timer::GetMSsinceLastFrame( )
{
	if( paused )
		return 0;
	else
		return current_time - next_last_time;
}

void Sim::Timer::SetPause( bool pause )
{
	paused = pause;
}
