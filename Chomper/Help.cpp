// This file defines the Help Screen
// Copyright Ian Finlayson 2005

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SimButton.h"
#include "SimTextWriter.h"

#include "Definitions.h"
#include "Functions.h"

Chomp::GameState Chomp::Help( SDL_Surface* screen )
{
	Sim::TextWriter tw( 3 );
	int AIRMOLE_24 = tw.AddFont( "CHdata/airmole.ttf", 24 );
	int AIRMOLE_72 = tw.AddFont( "CHdata/airmole.ttf", 72 );
	int SANS_16 = tw.AddFont( "CHdata/FreeSans.ttf", 16 );

	Sim::Button ok( 30, 530, 200, 50, 2, 250,200,0, 80, 8 );
	Sim::Button quit( 245, 530, 200, 50, 2, 250,200,0, 72, 8 );

	ok.SetBackGround( 255, 215, 45 );
	quit.SetBackGround( 255, 215, 45 );

	ok.SetForeGround( "Ok", tw.GetFont( AIRMOLE_24 ), 0,0,0 );
	quit.SetForeGround( "Quit", tw.GetFont( AIRMOLE_24 ), 0,0,0 );

	SDL_Event event;

	while( true )
	{
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
			{
				return Chomp::MENU;
			}
			else if( event.type == SDL_KEYDOWN )
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					return Chomp::MENU;
				}
			}
			else if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				ok.HandleMouseDown( event.button.x, event.button.y );
				quit.HandleMouseDown( event.button.x, event.button.y );
			}
			else if( event.type == SDL_MOUSEBUTTONUP )
			{
				if( ok.HandleMouseUp( event.button.x, event.button.y ) )
				{
					return Chomp::MENU;
				}

				if( quit.HandleMouseUp( event.button.x, event.button.y ) )
				{
					return Chomp::QUIT;
				}
			}
		}

		SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0,0,0 ) );

		tw.WriteText( screen, AIRMOLE_72, "Chomper Help", 15, 5, 255, 215, 45, 0, 0, 0 );
	
		tw.WriteText( screen, SANS_16, "Chomper is Pac-Man Clone. The playing area consists of a maze with three ghosts. These ghosts are not nice;", 15, 100, 255,255,255, 0,0,0 );
		tw.WriteText( screen, SANS_16, "they will kill you if you let them.  But don't lose hope! You have power pellets. If Chomper consumes a power", 15, 125, 255,255,255, 0,0,0 );
		tw.WriteText( screen, SANS_16, "pellet, the ghosts will turn blue, and you will be able to eat them! How the tables have turned! You also have", 15, 150, 255,255,255, 0,0,0 );
		tw.WriteText( screen, SANS_16, "speed burst pellets and extra life pellets. You must navigate the maze with the arrow keys, collecting every", 15, 175, 255,255,255, 0,0,0 );
		tw.WriteText( screen, SANS_16, "pellet as you go. If you make it fast enough, you will get a point bonus added to your score. Good Luck!", 15, 200, 255,255,255, 0,0,0 );
		tw.WriteText( screen, SANS_16, "Chomper is Copyright Ian Finlayson 2005, released under the MIT.  Enjoy!", 15, 225, 255,255,255, 0,0,0 );

		ok.Draw( screen );
		quit.Draw( screen );

		SDL_Flip( screen );
	}
}


