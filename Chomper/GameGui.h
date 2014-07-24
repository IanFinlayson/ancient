// This file separates the user interface stuff used
// by the game() function from the actual game code
// Copyright Ian Finlayson 2005

#ifndef GAME_GUI
#define GAME_GUI

namespace Chomp
{
	class GameGui
	{
	private:
		int AIRMOLE_24, FREE_SANS_14;
		Sim::TextWriter* tw;
		Sim::Button* pause;		bool paused;
		Sim::Button* menu;

		static const int TIME_ON_SPEED_BONUS = 3000;	// milliseconds
		static const int TIME_ON_POWER_BONUS = 5000;	// must be same as in Player.h

		int num_power_show;
		int num_speed_show;
		static const int MAX_THINGYS_TO_SHOW = 10;

		bool speed_bonus;	unsigned int time_speed_ends;
		bool power_bonus;	unsigned int time_power_ends;

		unsigned int pause_started;


	public:
		GameGui( );

		void PrepareForLevelScreen( SDL_Surface* screen, int level, int lev_score );
		void KillScreen( SDL_Surface* screen );

		void Draw( SDL_Surface* screen, int score, int lev_num, int lives );
		void HandleMouseDown( int mx, int my );

		int HandleMouseUp( int mx, int my );	//return value is what user did
												// (0:nothing) (1:pause) (2:unpause) (3:menu)
		void PowerBonusOn( );
		void SpeedBonusOn( );

		void Pause( );
		void Unpause( );
	};
}

#endif