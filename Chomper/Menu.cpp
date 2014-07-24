// This file defines the Main Menu
// Copyright Ian Finlayson 2005

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SimButton.h"
#include "SimTextWriter.h"

#include "Definitions.h"
#include "Functions.h"

Chomp::GameState Chomp::Menu( SDL_Surface* screen )
{
	Sim::TextWriter tw( 2 );
	int AIRMOLE_24 = tw.AddFont( "CHdata/airmole.ttf", 24 );
	int AIRMOLE_72 = tw.AddFont( "CHdata/airmole.ttf", 72 );

	Sim::Button game( 30, 130, 200, 50, 2, 250,200,0, 72, 8 );
	Sim::Button help( 30, 210, 200, 50, 2, 250,200,0, 72, 8 );
	Sim::Button quit( 30, 290, 200, 50, 2, 250,200,0, 72, 8 );

	game.SetBackGround( 255, 215, 45 );
	help.SetBackGround( 255, 215, 45 );
	quit.SetBackGround( 255, 215, 45 );

	game.SetForeGround( "Game", tw.GetFont( AIRMOLE_24 ), 0,0,0 );
	help.SetForeGround( "Help", tw.GetFont( AIRMOLE_24 ), 0,0,0 );
	quit.SetForeGround( "Quit", tw.GetFont( AIRMOLE_24 ), 0,0,0 );

	SDL_Event event;

	while( true )
	{
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
			{
				return Chomp::QUIT;
			}
			else if( event.type == SDL_KEYDOWN )
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					return Chomp::QUIT;
				}
			}
			else if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				game.HandleMouseDown( event.button.x, event.button.y );
				help.HandleMouseDown( event.button.x, event.button.y );
				quit.HandleMouseDown( event.button.x, event.button.y );
			}
			else if( event.type == SDL_MOUSEBUTTONUP )
			{
				if( game.HandleMouseUp( event.button.x, event.button.y ) )
				{
					return Chomp::GAME;
				}
				if( help.HandleMouseUp( event.button.x, event.button.y ) )
				{
					return Chomp::HELP;
				}
				if( quit.HandleMouseUp( event.button.x, event.button.y ) )
				{
					return Chomp::QUIT;
				}
			}
		}
	
		SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0,0,0 ) );

		tw.WriteText( screen, AIRMOLE_72, "Chomper", 20, 20, 250,200,0, 0,0,0 );

		game.Draw( screen );
		help.Draw( screen );
		quit.Draw( screen );

		SDL_Flip( screen );
	}
}
