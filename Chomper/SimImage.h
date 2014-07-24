/********************************************************************
*	SimImage.h					 Ian Finlayson 2005					*
*																	*
*	This class is a wrapper around SDL_Surface* providing ctors		*
*	and dtor, operator=, and some other stuff.						*
*																	*
********************************************************************/

#ifndef SIM_IMAGE
#define SIM_IMAGE

#include <SDL/SDL.h>

namespace Sim
{
	class Image
	{
	public:

		SDL_Surface* image;		//The image is public so no functionality is lost

		Image( );	//Blank
		Image( const char* filename );	//Loads it
		Image( const char* filename, int r_clear, int g_clear, int b_clear );	//Loads it and sets colorkey
		Image( SDL_Surface* other );	//Sets it to this surface
		Image( const Image & copy );	//Copy constructor
		~Image( );

		Image operator=( const Image & rhs );	//Overloaded =

		//These 2 functions are adapted from the SDL documentation
		Uint32 GetPixel( int x, int y );
		void SetPixel( int x, int y, Uint32 pixel );

		Image GetMirroredX( );
		Image GetMirroredY( );
	};
}
#endif
