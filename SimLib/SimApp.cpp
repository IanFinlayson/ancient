/********************************************************************
*	SimApp.cpp					 Ian Finlayson 2005					*
*																	*
*	Implementation of SimApp										*
*																	*
********************************************************************/

#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include "SimException.h"
#include "SimApp.h"

Sim::App::App( int w, int h, bool full, const char* title, const char* iconFile )
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		throw Sim::Exception( "App Constructor", "Could Not Initialize SDL" );
	}

	icon = NULL;
	if( iconFile )
	{
		icon = IMG_Load( iconFile );
		SDL_WM_SetIcon( icon, NULL );
	}

	int flags = SDL_SWSURFACE | SDL_ANYFORMAT;

	if( full )
	{
		flags |= SDL_FULLSCREEN;
	}

	screen = SDL_SetVideoMode( w, h, 32, flags );

	if( screen == NULL )
	{
		throw Sim::Exception( "App Constructor", "Video Mode Could not be set" );
	}

	if( title )
	{
		SDL_WM_SetCaption( title, NULL );
	}

    if( TTF_Init( ) != 0 )
	{
		throw Sim::Exception( "App Constructor", "Font Library Could not be Initialized" );
	}
}

Sim::App::~App( )
{
	if( icon )
	{
		SDL_FreeSurface( icon );
		icon = NULL;
	}

    TTF_Quit( );
	SDL_Quit( );
}