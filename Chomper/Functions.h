// Some functions called by main that
// I wanted to separate from the main code
// Copyright Ian Finlayson 2005

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

namespace Chomp
{
	// These functions only return when the player leaves their grips
	// i.e. they each have their own game loops. They return the game
	// state the application should go to next...

	GameState Intro( SDL_Surface* screen );
	GameState Menu( SDL_Surface* screen );
	GameState Game( SDL_Surface* screen, int* score, int* lev_num );
	GameState Won( SDL_Surface* screen, int score, bool won );
	GameState Help( SDL_Surface* screen );
}

#endif