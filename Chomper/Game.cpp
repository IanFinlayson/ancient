// This File defines the main game function
// This is where most of the work happens!
// Copyright Ian Finlayson 2005

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SimSprite.h"
#include "SimImage.h"
#include "SimTextWriter.h"
#include "SimButton.h"
#include "SimTimer.h"

#include "Definitions.h"
#include "Functions.h"
#include "Level.h"
#include "Player.h"
#include "GameGui.h"
#include "Ghost.h"
#include "Ghosties.h"

Chomp::GameState Chomp::Game( SDL_Surface* screen, int* score, int* lev_num )
{
	static Chomp::Level level;	// they are static so they are only created and 
								// initialized once no matter how many games are played
	static Chomp::Player player( &level );

	static Chomp::GameGui gui;

	static Chomp::Ghosties ghosts( &level, &player );

	Sim::Timer ti;

	player.ClearScore( );

	bool paused = false;

	int level_number = 1;

	level.Load( 1 );	//load level 1
	gui.PrepareForLevelScreen( screen, 1, 0 );

	//Now that level 1 is loaded, we must tell chomper + ghosts where to start...
	player.ResetChomper( );

	ghosts.Pause( );
	ghosts.Restart( );
	ghosts.Unpause( );

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
					return Chomp::MENU;
				}
				else if( event.key.keysym.sym == SDLK_RIGHT )
				{
					if( !paused )
						player.SetNextDirection( EAST );
				}
				else if( event.key.keysym.sym == SDLK_UP )
				{
					if( !paused )
						player.SetNextDirection( NORTH );
				}
				else if( event.key.keysym.sym == SDLK_LEFT )
				{
					if( !paused )
						player.SetNextDirection( WEST );
				}
				else if( event.key.keysym.sym == SDLK_DOWN )
				{
					if( !paused )
						player.SetNextDirection( SOUTH );
				}
				else if( event.key.keysym.sym == SDLK_RSHIFT )
				{
					if( !paused )
						player.SetNextDirection( NONE );
				}
			}
			else if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				gui.HandleMouseDown( event.button.x, event.button.y );
			}
			else if( event.type == SDL_MOUSEBUTTONUP )
			{
				int result = gui.HandleMouseUp( event.button.x, event.button.y );

				if( result == 1 )	//user paused
				{
					paused = true;

					level.Pause( );
					player.PauseChomper( );
					gui.Pause( );
					ghosts.Pause( );
				}
				else if( result == 2 )	//user unpaused
				{
					paused = false;

					level.Unpause( );
					player.UnpauseChomper( );
					gui.Unpause( );
					ghosts.Unpause( );

				}
				else if( result == 3 )	//user selected menu
				{
					return Chomp::MENU;
				}
			}
		}

		if( level.LevelDone( ) )
		{
			if( level_number >= NUM_LEVELS )
			{
				//User won game
				*score = ( player.GetScore( ) + level.GetScore( ) );
				return Chomp::WON;
			}
			else
			{
				level_number++;

				level.Load( level_number );

				player.PauseChomper( );
				ghosts.Pause( );

				int lev_score = level.GetScore( );
				player.AddToScore( lev_score );

				gui.PrepareForLevelScreen( screen, level_number, lev_score );

				player.UnpauseChomper( );
                player.ResetChomper( );
				ghosts.Restart( );
				ghosts.Unpause( );
				ghosts.Restart( );
			}
		}
		
		int result = player.Update( );

		if( result == 1 )	//power pellet
		{
			ghosts.TurnBlue( );	
			gui.PowerBonusOn( );
		}
		else if( result == 2 )	//speed bonus
		{
			gui.SpeedBonusOn( );
		}
		else if( result == 3 )	//Player has lost
		{
			*score = ( player.GetScore( ) + level.GetScore( ) );
			return Chomp::LOST;
		}

		int ghost_return = ghosts.Update( );

		if(  ghost_return == 1 )	//Chomper died
		{
			player.Kill( );

			if( player.GetLives( ) != 0 )
			{
				player.ResetChomper( );
				ghosts.Restart( );

				ghosts.Pause( );
				player.PauseChomper( );

				gui.KillScreen( screen );
				player.Update( );//This is done to fight the "Pause Effect"
				ghosts.Update( );

				//Must unpause now
				player.UnpauseChomper( );
				ghosts.Unpause( );
				player.ResetChomper( );
			}
			else
			{
				*score = ( player.GetScore( ) + level.GetScore( ) );
				return Chomp::LOST;
			}

		}
		else if( ghost_return == 2 ) //A ghost died
		{
			player.AddToScore( 50 );

		}

		ti.Update( );

		//The validate functions check if all entities are in bounds Hopefully whatever bugs
		//move them out of bounds will be found but ????
		ghosts.Validate( );
		player.Validate( );

		SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0,0,0 ) );

		gui.Draw( screen, player.GetScore(), level_number, player.GetLives() );
		level.Draw( screen );
		ghosts.Draw( screen );
		player.Draw( screen );

		SDL_Flip( screen );
	}
}

