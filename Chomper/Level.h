// This class defines the level used in Chomper
// note that this class is different from the 
// one used by the level editor.
// Copyright Ian Finlayson 2005

#ifndef LEVEL_H
#define LEVEL_H

namespace Chomp
{
	class Level
	{
	private:
		struct LevelData
		{
			unsigned char tiles[ NUM_TILES_PER_ROW * NUM_TILES_PER_ROW ];

			unsigned char chomper_start;
			unsigned char ghostys_start;
		};

		LevelData level_data;		// Actual binary data that composes level
		Sim::Image* level_image;	// Image used for level

		SDL_Rect rects[ 20 ];		// "frames" of the level

		static const int STARTING_POINTS			= 2000;
		static const int NUMBER_REMOVED_PER_SECOND	= 20;
		unsigned int timer;
		int point_bonus;
		bool paused;

	public:
		Level( );
		~Level( );

		void Load( int level_number );
		void Draw( SDL_Surface* screen );

		bool CanGo( int x, int y, Direction dir );
		void GetChomperStart( int* x, int* y );
		void GetGhostysStart( int* x, int* y );

		bool LevelDone( );

		void Pause( );
		void Unpause( );

		unsigned char GetData( int x, int y );

		int GetScore( );	//Calling this also resets score

		int HandlePellet( int x, int y );	//Returns the type of pellet (0:nothing) (1:pellet) (2:power_pellet)
											//							 (3:speed_pellet) (4:life_pellet)
	};
}

#endif