// Implementation of the Level methods
// Copyright Ian Finlayson 2005

#include <SDL/SDL.h>
#include <fstream>

#include "SimImage.h"

#include "Definitions.h"
#include "Level.h"

Chomp::Level::Level( )
{
	level_image = new Sim::Image( "CHdata/level.png", 0,0,255 );

	for( int i=0; i<20; i++ )
	{
		rects[i].x = i*TILE_SIZE;
		rects[i].y = 0;
		rects[i].w = TILE_SIZE;
		rects[i].h = TILE_SIZE;
	}

	timer = SDL_GetTicks( );
	point_bonus = STARTING_POINTS;
	paused = false;
}

Chomp::Level::~Level( )
{
	delete level_image;
}

void Chomp::Level::Load( int level_number )
{
	std::fstream file;
	char file_name[20];

	sprintf( file_name, "CHdata/level%i.chl", level_number );

	file.open( file_name, std::ios::in | std::ios::binary );

	file.read( reinterpret_cast< char* >( &level_data ), sizeof( level_data ) );

	file.close( );
}

void Chomp::Level::Draw( SDL_Surface* screen )
{
	SDL_Rect dest;

	if( !paused )
	{
		if( ( SDL_GetTicks( ) - timer ) > 1000 )
		{
			timer = SDL_GetTicks( );
			point_bonus -= NUMBER_REMOVED_PER_SECOND;
		}
	}

	for( int i=0; i<NUM_TILES_PER_ROW; i++ )
	{
		for( int j=0; j<NUM_TILES_PER_ROW; j++ )	//for every tile
		{
			unsigned char temp = level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x0f;

			dest.x = j * TILE_SIZE;
			dest.y = i * TILE_SIZE;
			dest.w = TILE_SIZE;
			dest.h = TILE_SIZE;

			SDL_BlitSurface( level_image->image, rects + temp, screen, &dest );	//blit the tiles


			if( level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x10 )	// blit the pellet
			{
				SDL_BlitSurface( level_image->image, rects + 16, screen, &dest );
			}

			else if( level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x20 ) // blit power pellet
			{
				SDL_BlitSurface( level_image->image, rects + 17, screen, &dest );
			}

			else if( level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x40 ) // blit speed bonus
			{
				SDL_BlitSurface( level_image->image, rects + 18, screen, &dest );
			}

			else if( level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x80 ) // blit point bonus pellet
			{
				SDL_BlitSurface( level_image->image, rects + 19, screen, &dest );
			}
		}
	}
}

bool Chomp::Level::CanGo( int x, int y, Direction dir )
{
	if( dir == EAST )
	{
		if( level_data.tiles[ y*NUM_TILES_PER_ROW + x ] & 0x02 )
			return true;
		else
			return false;

	}
	else if( dir == NORTH )
	{
		if( level_data.tiles[ y*NUM_TILES_PER_ROW + x ] & 0x01 )
			return true;
		else
			return false;
	}
	else if( dir == WEST )
	{
		if( level_data.tiles[ y*NUM_TILES_PER_ROW + x ] & 0x08 )
			return true;
		else
			return false;
	}
	else
	{
		if( level_data.tiles[ y*NUM_TILES_PER_ROW + x ] & 0x04 )
			return true;
		else
			return false;
	}
}

void Chomp::Level::GetChomperStart( int* x, int* y )
{
	unsigned char start = level_data.chomper_start;

	*x = ( (start % NUM_TILES_PER_ROW) * TILE_SIZE ) + 4;

	*y = 0;
	while( start > NUM_TILES_PER_ROW )
	{
		(*y)++;
		start -= NUM_TILES_PER_ROW;
	}

	(*y) *= TILE_SIZE;

	(*y) += 4;
}

void Chomp::Level::GetGhostysStart( int* x, int* y )
{
	unsigned char start = level_data.ghostys_start;

	// Convert from tiles to pixels...
	*x = ( (start % NUM_TILES_PER_ROW) * TILE_SIZE ) + 4;

	*y = 0;
	while( start > NUM_TILES_PER_ROW )
	{
		(*y)++;
		start -= NUM_TILES_PER_ROW;
	}

	(*y) *= TILE_SIZE;

	(*y) += 4;
}

int Chomp::Level::HandlePellet( int x, int y )
{
	x = x / TILE_SIZE;
	y = y / TILE_SIZE;

	unsigned char* tile = &level_data.tiles[ y*NUM_TILES_PER_ROW + x ];

	if( *tile & 0x10 )	//normal pellet
	{
		*tile &= 0x0f;
		return 1;
	}

	if( *tile & 0x20 )	//power pellet
	{
		*tile &= 0x0f;
		return 2;
	}

	if( *tile & 0x40 ) //speed pellet
	{
		*tile &= 0x0f;
		return 3;
	}

	if( *tile & 0x80 ) //life pellet
	{
		*tile &= 0x0f;
		return 4;
	}

    return 0;	//nothing
}

bool Chomp::Level::LevelDone( )
{
	for( int i=0; i<NUM_TILES_PER_ROW; i++ )
	{
		for( int j=0; j<NUM_TILES_PER_ROW; j++ )
		{
			if( level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0xf0 )
			{
				return false;
			}
		}
	}

	return true;
}

int Chomp::Level::GetScore( )
{
	int score = point_bonus;

	point_bonus = STARTING_POINTS;

	if( score > 0 )
	{
		return score;
	}
	else
	{
		return 0;
	}
}

void Chomp::Level::Pause( )
{
	paused = true;
}

void Chomp::Level::Unpause( )
{
	paused = false;
}

unsigned char Chomp::Level::GetData( int x, int y )
{
	return level_data.tiles[ y*NUM_TILES_PER_ROW + x ];
}
