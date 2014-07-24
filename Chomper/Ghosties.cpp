// Ghosties.cpp Implements the Ghosties class
// Copyright Ian Finlayson 2005

#include <SDL/SDL.h>

#include "SimSprite.h"
#include "SimImage.h"
#include "SimException.h"

#include "Definitions.h"
#include "Level.h"
#include "Player.h"
#include "Ghost.h"
#include "Ghosties.h"

Chomp::Ghosties::Ghosties( Level* level, Player* player )
{
	ghost_img = new Sim::Image( "CHdata/ghosts.png", 0, 255, 0 );
	SDL_SetAlpha( ghost_img->image, SDL_SRCALPHA | SDL_RLEACCEL, 185 );

	pupil_img = new Sim::Image( "CHdata/pupils.png", 0, 255, 0 );

	for( int i=0; i<3; i++ )
	{
		ghosts[i] = new Ghost( 0, 0, ghost_img->image, pupil_img->image, i, level, player );
	}

	this->level = level;
}

Chomp::Ghosties::~Ghosties( )
{
	for( int i=0; i<3; i++ )
	{
		delete ghosts[i];
	}

	delete ghost_img;
	delete pupil_img;
}

void Chomp::Ghosties::Restart( )
{
	int x,y;

	level->GetGhostysStart( &x, &y );
	
	for( int i=0; i<3; i++ )
	{
		ghosts[i]->SetPosition( x, y );
		ghosts[i]->Restart( );
	}
}


int Chomp::Ghosties::Update( )	//1 is chomper died, 2 is ghost died
{
	int val = 0;

	for( int i=0; i<3; i++ )
	{
		int up = ghosts[i]->Update( );

		if( up == 1 )
		{
			val = 1;
		}
		else if( up == 2 )
		{
			if( val == 0 )	//Only set it if chomper has not died
			{
				val = 2;
			}
		}
	}

	return val;
}

void Chomp::Ghosties::Draw( SDL_Surface* screen )
{
	for( int i=0; i<3; i++ )
	{
		ghosts[i]->Draw( screen );
	}
}

void Chomp::Ghosties::Pause( )
{
	for( int i=0; i<3; i++ )
	{
		ghosts[i]->Pause( );
	}
}

void Chomp::Ghosties::Unpause( )
{
	for( int i=0; i<3; i++ )
	{
		ghosts[i]->Unpause( );
	}
}

void Chomp::Ghosties::TurnBlue( )
{
	for( int i=0; i<3; i++ )
	{
		ghosts[i]->TurnBlue( );
	}
}

void Chomp::Ghosties::Validate( )
{
	for( int i=0; i<3; i++ )
	{
		ghosts[i]->Validate( );
	}
}
