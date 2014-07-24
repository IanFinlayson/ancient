/********************************************************************
*	SimApp.h					 Ian Finlayson 2005					*
*																	*
*	This class wraps up some SDL stuff (mostly so we can initialize	*
*	SDL before main so our global var's constructors work)			*
*																	*
********************************************************************/

#ifndef SIM_APP
#define SIM_APP

#include <SDL/SDL.h>

namespace Sim
{
	class App
	{
	private:
		SDL_Surface* icon;

	public:
		SDL_Surface* screen;

		App( int w, int h, bool full, const char* title, const char* iconFile );

		inline void Flip( )
		{
			SDL_Flip( screen );
		}

		inline void Clear( int r, int g, int b )
		{
			SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, r, g, b ) );
		}

		~App( );
	};
}

#endif
