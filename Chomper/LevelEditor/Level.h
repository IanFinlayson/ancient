// The level class for use in Chomper and the Chomper Level editor

#ifndef LEVEL_H
#define LEVEL_H


class Level
{
private:
	
	static const int NUM_TILES_PER_ROW = 15;
	static const int TILE_SIZE = 40;

	struct LevelData
	{
		unsigned char tiles[ NUM_TILES_PER_ROW * NUM_TILES_PER_ROW ];

		unsigned char chomper_start;
		unsigned char ghostys_start;
	};	// I'm counting on unsigned char to be 1 byte

	SDL_Surface* level_image;

	SDL_Rect rects[ 16 ];		// The frames of the tiles
	SDL_Rect pellet;		// frame of image with pellet
	SDL_Rect pow_pellet;	// frame of image with power pellet
	SDL_Rect speed_burst;	// frame of image with speed bonus
	SDL_Rect wall_smash;	// frame of image with wall smash bonus

	LevelData   level_data;		// The actual data

public:
	Level( );
	~Level( );

	void Load( const char* level_name );
	void Save( const char* level_name );

	void SetPiece( int x, int y, unsigned char piece );

	void SetBonus( int x, int y, int bonus );

	void SetChomperStart( int x, int y );
	void SetGhostysStart( int x, int y );

	unsigned char GetPiece( int x, int y );

	void Validate( );

	void Draw( SDL_Surface* screen );
};

#endif


