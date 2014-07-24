// Main Application file for Chomper
// Copyright Ian Finlayson 2005

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SimApp.h"

#include "Definitions.h"
#include "Functions.h"


int main( int argc, char** argv )
{
	Sim::App app( Chomp::WIDTH, Chomp::HEIGHT, Chomp::FULLSCREEN, "Chomper", "CHdata/icon.png" );

	Chomp::GameState game_state = Chomp::INTRO;

	int score, lev_num;

	while( game_state != Chomp::QUIT )		//While user does not quit
	{
		if( game_state == Chomp::GAME )
		{
			game_state = Chomp::Game( app.screen, &score, &lev_num );
		}
		else if( game_state == Chomp::MENU )
		{
			game_state = Chomp::Menu( app.screen );
		}
		else if( game_state == Chomp::INTRO )
		{
			game_state = Chomp::Intro( app.screen );
		}
		else if( game_state == Chomp::WON )
		{
			game_state = Chomp::Won( app.screen, score, true );
		}
		else if( game_state == Chomp::LOST )
		{
			game_state = Chomp::Won( app.screen, score, false );
		}
		else if( game_state == Chomp::HELP )
		{
			game_state = Chomp::Help( app.screen );
		}
	}

	return 0;
}
