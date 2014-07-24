// Ghost.h Defines a single ghost
// (Ghosties declares 3 ghosts and interfaces them wiht game)
// Copyright Ian Finlayson 2005

#ifndef GHOST_H
#define GHOST_H

namespace Chomp
{
	class Ghost
	{
	private:
		Sim::Sprite* sprite;	//Contains most everyting ghost needs
		int ghost_color;		//Lets ghost know which ghost he is...

		struct Pupil
		{
			SDL_Surface* img;	//Each ghost just has a copy
			SDL_Rect location;
		};

		Level* level;	//A pointer to the level
		Player* player;	//A pointer to the player

		Pupil pupil;

		// AI kind of works like finite state machine...
		typedef int AIState;

		static const AIState BASIC		= 1;
		static const AIState AVOID		= 2;
		static const AIState PURSUIT	= 3;

		AIState current_state;

		//Basically make up the AI tell which direction to move
		Direction GetNextRed( );
		Direction GetNextYellow( );
		Direction GetNextPurple( );

		Direction next_direction;

		float speed;
		static const int NORMAL_SPEED = 140;
		static const int BLUE_SPEED = 95;

		static const int TIME_BLUE = 5000;

		unsigned int time_blue_ends;

		bool is_blue;
		bool paused;
		unsigned int time_pause_started;

		bool dead;

		float dx_saved, dy_saved;
		Direction last;


	public:
		// ghost_color: 0 : red		1 : yellow		2 : purple
		Ghost( int x, int y, SDL_Surface* ghost_img, SDL_Surface* pupil_img, int ghost_color, Level* the_level, Player* the_player );
		~Ghost( );

		void Draw( SDL_Surface* screen );
		int Update( );	//Returns 1 if Chomper got killed, 2 if Ghost got killed

		void SetPosition( int x, int y );

		void TurnBlue( );

		void Restart( );

		void Pause( );
		void Unpause( );

		void Validate( );

	};
}

#endif
