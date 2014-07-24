// Ghosties.h interfaces 3 ghosts with the rest of the application
// -I did not plan it all out very well-
// Copyright Ian Finlayson 2005

#ifndef GHOSTIES_H
#define GHOSTIES_H

namespace Chomp
{
	class Ghosties
	{
	private:
		Ghost* ghosts[3];	//The three ghosts

		Sim::Image* ghost_img;
		Sim::Image* pupil_img;

		Level* level;

	public:
		Ghosties( Level* level, Player* player );
		~Ghosties( );

		void Restart( );
		void Pause( );
		void Unpause( );
		int Update( );	//Returns 1 if Chomper got killed, 2 if ghost got killed
		void Draw( SDL_Surface* screen );
		void Validate( );

		void TurnBlue( );

	};
}
#endif
