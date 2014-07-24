// This class defines the player including the
// animation, position, score etc
// Copyright Ian Finlayson 2005

#ifndef PLAYER_H
#define PLAYER_H

namespace Chomp
{
	class Player
	{
	private:
		static const int NUM_ANIMATIONS			= 4;
		static const int NUM_FRAMES_PER_ANIM	= 4;
		static const int FRAME_RATE				= 100;	//ms between animations

		//For some reason the compiler made me initialize these in constructor?
		float NORMAL_SPEED;	//pixels per second
		float BONUS_SPEED;	//after speed bonus

		static const int STARTING_LIVES = 5;

		static const int TIME_ON_BONUS_SPEED = 3000;	//milliseconds
		static const int TIME_ON_POWER_BONUS = 5000;

		unsigned int time_bonus_ends;
		bool on_bonus_speed;

		unsigned int time_of_pause;
		bool paused;
	
		// Use Chomp::Direction to set the animation - they correspond!

		Sim::Sprite* sprite;
		Sim::Image* foreward;
		Sim::Image* backward;

		Direction next_direction;

		Direction last_direction;	//Used in unpausing

		Level* level;	//Player needs to know what level is like

		static const int POINTS_PER_PELLET	= 5;
		static const int POINTS_PER_BONUS	= 10;
		int points;

		int lives;

	public:
		Player( Level* lev );
		~Player( );

		void ResetChomper( );

		void UnpauseChomper( );
		void PauseChomper( );

		void SetNextDirection( Direction dir );
		int Update( );		//returns 0 on no action 1 on power pellet and 2 on speed pellet, 3 on lost
		void Draw( SDL_Surface* screen );

		void ClearScore( );
		void AddToScore( int sco );

		Direction GetHeading( );

		int GetScore( );
		int GetLives( );

		void Validate( );

		void Kill( );

		SDL_Rect GetPosition( ){ return sprite->GetPosition( ); }
	};
}

#endif