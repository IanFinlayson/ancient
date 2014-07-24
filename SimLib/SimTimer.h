/********************************************************************
*	SimTimer.h					 Ian Finlayson 2005					*
*																	*
*	This class provides an abstract way to get fps etc.				*
*																	*
********************************************************************/

#ifndef SIM_TIMER
#define SIM_TIMER

namespace Sim
{
	class Timer
	{
	private:
		unsigned int start_time;

		unsigned int next_last_time;
		unsigned int current_time;
		
		unsigned int total_frames;

		bool paused;

	public:
		Timer( );

		void Update( );
		unsigned int GetFPS( );
		unsigned int GetMSsinceLastFrame( );
		void SetPause( bool pause );		// This will make it so GetMSsinceLastFrame returns 0 while paused..
											// You won't have to pause all your stuff when pausing the game,
											// Just pause the timer and pass the elapsed time like normal...
	};
}

#endif