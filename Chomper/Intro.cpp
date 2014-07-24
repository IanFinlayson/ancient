// This file defines the Intro scene
// Copyright Ian Finlayson 2005

#include <SDL/SDL.h>

#include "SimImage.h"

#include "Definitions.h"
#include "Functions.h"

Chomp::GameState Chomp::Intro( SDL_Surface* screen )
{
	unsigned int TIME_FOR_FADE = 1500;
	unsigned int TIME_FOR_STAY = 1500;

	//If we are in fullscreen mode, we must wait for the screen to set???
	if( Chomp::FULLSCREEN )
	{
		SDL_Delay( 1500 );
	}

	Sim::Image logo( "CHdata/logo.png" );

	SDL_Rect dest;
	dest.x = 141;
	dest.y = 100;
	dest.w = logo.image->w;
	dest.h = logo.image->h;

	unsigned int time_end = SDL_GetTicks( ) + TIME_FOR_FADE;

	while( time_end > SDL_GetTicks( ) )	//Fade in
	{
		float per_left =( (float)SDL_GetTicks() - time_end ) /(float)TIME_FOR_FADE;

		SDL_SetAlpha( logo.image, SDL_SRCALPHA | SDL_RLEACCEL, per_left*255 );

		SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );
		SDL_BlitSurface( logo.image, NULL, screen, &dest );
		SDL_Flip( screen );
	}

	// Do the stay part...
	SDL_SetAlpha( logo.image, SDL_SRCALPHA | SDL_RLEACCEL, 255 );
	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );
	SDL_BlitSurface( logo.image, NULL, screen, &dest );
	SDL_Flip( screen );
	SDL_Delay( TIME_FOR_STAY );


	time_end = SDL_GetTicks( ) + TIME_FOR_FADE;

	while( time_end > SDL_GetTicks( ) )	//Fade out
	{
		float per_done = 1.0f - ( ( (float)SDL_GetTicks() - time_end ) /(float)TIME_FOR_FADE );

		SDL_SetAlpha( logo.image, SDL_SRCALPHA | SDL_RLEACCEL, per_done*255 );

		SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );
		SDL_BlitSurface( logo.image, NULL, screen, &dest );
		SDL_Flip( screen );
	}

	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0,0,0 ) );
	SDL_Flip( screen );

	return Chomp::MENU;
}



