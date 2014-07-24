// This file gives many global definitions
// Used in Chomper. Copyright Ian Finlayson 2005

#ifndef DEFINES_H
#define DEFINES_H

namespace Chomp
{
	typedef int Direction;
	const Direction NONE	= -1;
	const Direction EAST	= 0;
	const Direction NORTH	= 1;
    const Direction WEST	= 2;
	const Direction SOUTH	= 3;

	typedef int GameState;
	const GameState QUIT	= 0;
	const GameState INTRO	= 1;
	const GameState MENU	= 2;
	const GameState GAME	= 3;
	const GameState WON		= 4;
	const GameState LOST	= 5;
	const GameState HELP	= 6;

	const int NUM_TILES_PER_ROW		= 15;
	const int TILE_SIZE				= 40;

	const int WIDTH			= 800;
	const int HEIGHT		= 600;
	const int FULLSCREEN	= 0;

	const int NUM_LEVELS	= 5;

	bool Collide( SDL_Rect*, SDL_Rect* );	//Defined here for no good reason
}
#endif
