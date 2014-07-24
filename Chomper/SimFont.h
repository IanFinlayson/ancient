/********************************************************************
*	SimFont.h					 Ian Finlayson 2005					*
*																	*
*	This class is thin wrapper over TTF_Font						*
********************************************************************/

#ifndef SIM_FONT
#define SIM_FONT

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

namespace Sim
{
	class Font
	{
	public:
		TTF_Font* font;

		Font( char* file_name, int size )
		{
			font = TTF_OpenFont( file_name, size );

			if( font == NULL )
			{
				throw Sim::Exception( "Font Constructor", "Font could not load" );
			}
		}
		~Font( )
		{
			if( font )
			{
				TTF_CloseFont( font );
			}
		}
	};
}
#endif