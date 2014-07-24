// Implements the level stuff


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <fstream>

#include "Level.h"

Level::Level( )
{
	SDL_Surface* temp = IMG_Load( "CHdata/level.png" );
	
	SDL_SetColorKey( temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB( temp->format, 0,0,255 ) );

	level_image = SDL_DisplayFormat( temp );

	SDL_FreeSurface( temp );

	for( int i=0; i<16; i++ )
	{
		rects[i].x = i*TILE_SIZE;
		rects[i].y = 0;
		rects[i].h = TILE_SIZE;
		rects[i].w = TILE_SIZE;
	}

	pellet.h = pellet.w = pow_pellet.h = pow_pellet.w = speed_burst.h = speed_burst.w = wall_smash.h = wall_smash.w = TILE_SIZE;
	pellet.y = pow_pellet.y = speed_burst.y = wall_smash.y = 0;

	pellet.x = 16*TILE_SIZE;
	pow_pellet.x = 17*TILE_SIZE;
	speed_burst.x = 18*TILE_SIZE;
	wall_smash.x = 19*TILE_SIZE;


//////////////////////////////////This section should be taken out for program

	for( int i=0; i< (NUM_TILES_PER_ROW*NUM_TILES_PER_ROW); i++ )
	{
		level_data.tiles[i] = 0x0f;	//Blank them all out
	}

	level_data.chomper_start = level_data.ghostys_start = 0;
}

Level::~Level( )
{
	if( level_image )
	{
		SDL_FreeSurface( level_image );
	}
}


void Level::Load( const char* level_name )
{
	std::fstream file;

	file.open( level_name, std::ios::in | std::ios::binary );

	file.read( reinterpret_cast< char* >( &level_data ), sizeof( level_data ) );

	file.close( );
}

void Level::Save( const char* level_name )
{
	std::fstream file;

	file.open( level_name, std::ios::out | std::ios::binary );

	file.write( reinterpret_cast< char* >( &level_data ), sizeof( level_data ) );

	file.close( );
}

void Level::Draw( SDL_Surface* screen )
{
	SDL_Rect dest;

	for( int i=0; i<NUM_TILES_PER_ROW; i++ )
	{
		for( int j=0; j<NUM_TILES_PER_ROW; j++ )
		{
			unsigned char temp = level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x0f;

			dest.w = dest.h = TILE_SIZE;
			dest.y = i * TILE_SIZE;
			dest.x = j * TILE_SIZE;

			SDL_BlitSurface( level_image, rects + temp, screen, &dest );	//blit the tiles

			if( level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x10 )	// blit the pellet
			{
				SDL_BlitSurface( level_image, &pellet, screen, &dest );
			}

			else if( level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x20 ) // blit power pellet
			{
				SDL_BlitSurface( level_image, &pow_pellet, screen, &dest );
			}

			else if( level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x40 ) // blit speed bonus
			{
				SDL_BlitSurface( level_image, &speed_burst, screen, &dest );
			}

			else if( level_data.tiles[ i*NUM_TILES_PER_ROW + j ] & 0x80 ) // blit opower pellet
			{
				SDL_BlitSurface( level_image, &wall_smash, screen, &dest );
			}
		}
	}
}

void Level::SetPiece( int x, int y, unsigned char piece )
{
	unsigned char curr = level_data.tiles[ (y * NUM_TILES_PER_ROW) + x ];

	unsigned char new_piece = ( curr & 0xf0 ) | ( piece & 0x0f );

	level_data.tiles[ (y * NUM_TILES_PER_ROW) + x ] = new_piece;
}

void Level::SetBonus( int x, int y, int bonus )
{
	// 16 pellet
	// 17 power pellet
	// 18 speed pellet
	// 19 wall pellet
	// 20 no pellet

	unsigned char curr = level_data.tiles[ (y * NUM_TILES_PER_ROW) + x ];
	
	unsigned char new_piece;

	if( bonus == 16 )
		new_piece = (curr & 0x0f) | 0x10;
	
	else if( bonus == 17 )
		new_piece = (curr & 0x0f) | 0x20;

	else if( bonus == 18 )
		new_piece = (curr & 0x0f) | 0x40;

	else if( bonus == 19 )
		new_piece = (curr & 0x0f) | 0x80;

	else
		new_piece = (curr & 0x0f);

	level_data.tiles[ (y * NUM_TILES_PER_ROW) + x ] = new_piece;
}

void Level::SetChomperStart( int x, int y )
{
	level_data.chomper_start = (y * NUM_TILES_PER_ROW) + x;
}

void Level::SetGhostysStart( int x, int y )
{
	level_data.ghostys_start = (y * NUM_TILES_PER_ROW) + x;
}

unsigned char Level::GetPiece( int x, int y )
{
	return level_data.tiles[ (y * NUM_TILES_PER_ROW) + x ];
}


void Level::Validate( )
{
	// This function makes sure the level is alligned properly

	for( int i=0; i<NUM_TILES_PER_ROW; i++ )
	{
		for( int j=0; j<NUM_TILES_PER_ROW; j++ )	// For each tile
		{
			unsigned char* curr = &level_data.tiles[ i*NUM_TILES_PER_ROW + j ];

			if( j == (NUM_TILES_PER_ROW - 1) )	//if block is on right edge
			{
				// Make sure edge is filled in
				*curr = *curr & 0xfd;
			}
			else
			{
				// Make sure if curr can't go right, right can't go left
				if( !(*curr & 0x02) )
				{
					level_data.tiles[ i*NUM_TILES_PER_ROW + j + 1 ] = level_data.tiles[ i*NUM_TILES_PER_ROW + j + 1 ] & 0xf7;
				}
			}

			if( j == 0 )	//if block is on left edge
			{
				// Make sure edge is filled in
				*curr = *curr & 0xf7;
			}
			else
			{
				// Make sure if curr can't go left, left can't go right
				if( !(*curr & 0x08) )
				{
					level_data.tiles[ i*NUM_TILES_PER_ROW + j - 1 ] = level_data.tiles[ i*NUM_TILES_PER_ROW + j - 1 ] & 0xfd;
				}
			}

			if( i == (NUM_TILES_PER_ROW - 1) )	//if block is on bottom edge
			{
				// Make sure edge is filled in
				*curr = *curr & 0xfb;
			}
			else
			{
				// Make sure if curr can't go down, down can't go up
				if( !(*curr & 0x04) )
				{
					level_data.tiles[ (i+1)*NUM_TILES_PER_ROW + j ] = level_data.tiles[ (i+1)*NUM_TILES_PER_ROW + j ] & 0xfe;
				}
			}

			if( i == 0 )	// if block is on top edge
			{
				// Make sure edge is filled in
				*curr = *curr & 0xfe;
			}
			else
			{
				// Make sure if curr can't up, up can't go down
				if( !(*curr & 0x01) )
				{
					level_data.tiles[ (i-1)*NUM_TILES_PER_ROW + j ] = level_data.tiles[ (i-1)*NUM_TILES_PER_ROW + j ] & 0xfb;
				}
			}
		}
	}
}






