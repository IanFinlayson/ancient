/********************************************************************
*	SimImage.cpp				 Ian Finlayson 2005					*
*																	*
*	Implements SimImage												*
*																	*
********************************************************************/

#include <SDL/SDL_image.h>

#include "SimException.h"
#include "SimImage.h"

Sim::Image::Image( )
{
	image = NULL;
}

Sim::Image::Image( const char* fileName )
{
	SDL_Surface* temp = IMG_Load( fileName );

	if( temp == NULL )
	{
		throw Sim::Exception( "Image Constructor", "Could Not Load" );
	}

	image = SDL_DisplayFormat( temp );

	SDL_FreeSurface( temp );
}

Sim::Image::Image( const char* fileName, int r_clear, int g_clear, int b_clear )
{
	SDL_Surface* temp = IMG_Load( fileName );

	if( temp == NULL )
	{
		throw Sim::Exception( "Image Constructor", "Could Not load" );
	}
	
	SDL_SetColorKey( temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB( temp->format, r_clear, g_clear, b_clear ) );

	image = SDL_DisplayFormat( temp );

	SDL_FreeSurface( temp );

}

Sim::Image::Image( SDL_Surface* other )
{
	image = SDL_DisplayFormat( other );
}

Sim::Image::Image( const Image & copy )
{
	image = SDL_DisplayFormat( copy.image );
}

Sim::Image::~Image( )
{
	if( image )
	{
		SDL_FreeSurface( image );
	}
}

Sim::Image Sim::Image::operator=( const Image & rhs )
{
	if( image )
	{
		SDL_FreeSurface( image );
	}

	image = SDL_DisplayFormat( rhs.image );

	return *this;
}

// This function comes from the SDL documentation
// I modified it a little bit
void Sim::Image::SetPixel( int x, int y, Uint32 pixel )
{
	// Lock the surface if we must
	if( SDL_MUSTLOCK( image ) )
	{
		SDL_LockSurface( image );
	}

	int bpp = image->format->BytesPerPixel;

	Uint8 *p = (Uint8 *)image->pixels + y * image->pitch + x * bpp;

	if( bpp == 1 )
	{
		*p = pixel;
	}
	else if( bpp == 2 )
	{
		*(Uint16 *)p = pixel;
	}
	else if( bpp == 3 )
	{
		if( SDL_BYTEORDER == SDL_BIG_ENDIAN )
		{
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else
		{
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
	}
	else if( bpp == 4 )
	{
		*(Uint32 *)p = pixel;
	}
	else
	{
		throw Sim::Exception( "SetPixel Function", "Too many bytes per pixel" );
	}

	if( SDL_MUSTLOCK( image ) )
	{
		SDL_UnlockSurface( image );
	}
}

// This function comes from the SDL documentation
// I modified it a little bit
Uint32 Sim::Image::GetPixel( int x, int y )
{
	if( SDL_MUSTLOCK( image ) )
	{
		SDL_LockSurface( image );
	}

	int bpp = image->format->BytesPerPixel;

	Uint8 *p = (Uint8 *)image->pixels + y * image->pitch + x * bpp;

	if( bpp == 1 )
	{
		return *p;
	}
	else if( bpp == 2 )
	{
		return *(Uint16 *)p;
	}
	else if( bpp == 3 )
	{
		if( SDL_BYTEORDER == SDL_BIG_ENDIAN )
		{
			return p[0] << 16 | p[1] << 8 | p[2];
		}
		else
		{
			return p[0] | p[1] << 8 | p[2] << 16;
		}
	}
	else if( bpp == 4 )
	{
		return *(Uint32 *)p;
	}
	else
	{
		throw Sim::Exception( "SetPixel Function", "Too many bytes per pixel" );
	}

	if( SDL_MUSTLOCK( image ) )
	{
		SDL_UnlockSurface( image );
	}
}

Sim::Image Sim::Image::GetMirroredX( )
{
	Sim::Image newImage( SDL_CreateRGBSurface( SDL_HWSURFACE, image->w, image->h, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask) );


	for( int y = 0; y < image->h; y++ )
	{
		for( int x = 0; x < image->w; x++ )
		{
			newImage.SetPixel( x, y, this->GetPixel( image->w - x - 1, y ) );
		}
	}

	SDL_SetColorKey( newImage.image, SDL_SRCCOLORKEY | SDL_RLEACCEL, this->image->format->colorkey );

	return newImage;
}

Sim::Image Sim::Image::GetMirroredY( )
{
	Sim::Image newImage( SDL_CreateRGBSurface( SDL_HWSURFACE, image->w, image->h, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask) );

	for( int y = 0; y < image->h; y++ )
	{
		for( int x = 0; x < image->w; x++ )
		{
			newImage.SetPixel( x, y, this->GetPixel( x, image->h - y - 1 ) );
		}
	}

	SDL_SetColorKey( newImage.image, SDL_SRCCOLORKEY | SDL_RLEACCEL, this->image->format->colorkey );

	return newImage;
}
