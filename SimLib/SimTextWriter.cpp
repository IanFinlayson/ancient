/********************************************************************
*	SimTextWriter.cpp			 Ian Finlayson 2005					*
*																	*
*	Implements the text writer										*
*																	*
********************************************************************/

#include "SimTextWriter.h"
#include "SimException.h"

Sim::TextWriter::TextWriter( int numFonts )
{
	num_fonts = numFonts;

	fonts = new TTF_Font*[ num_fonts ];

	curr_font = -1;
}

Sim::TextWriter::~TextWriter( )
{
	for( int i=0; i<num_fonts; i++ )
	{
		if( fonts[i] != NULL )
			TTF_CloseFont( fonts[i] );
	}

	delete [] fonts;
}

int Sim::TextWriter::AddFont( const char* font_name, int size )
{
	curr_font++;

	if( curr_font >= num_fonts )
	{
		throw Sim::Exception( "AddFont", "Attempt to add too many fonts!" );
	}

	fonts[ curr_font ] = TTF_OpenFont( font_name, size );

	if( fonts[ curr_font ] == NULL )
	{
		throw Sim::Exception( "AddFont", "Font can't load" );
	}

	return( curr_font );
}

TTF_Font* Sim::TextWriter::GetFont( int font_index )
{
	return fonts[ font_index ];
}

void Sim::TextWriter::WriteText( SDL_Surface* screen, int font_index, const char* text, int x, int y, int fr, int fg, int fb )
{
	SDL_Color fore;	fore.r = fr;	fore.g = fg;	fore.b = fb;

	SDL_Rect dest;	dest.x = x;		dest.y = y;		dest.h = dest.w = 0;

	SDL_Surface* temp = TTF_RenderText_Blended( fonts[ font_index ], text, fore );

	SDL_BlitSurface( temp, NULL, screen, &dest );

	SDL_FreeSurface( temp );
}

void Sim::TextWriter::WriteText( SDL_Surface* screen, int font_index, const char* text, int x, int y, int fr, int fg, int fb, int br, int bg, int bb )
{
	SDL_Color fore;	fore.r = fr;	fore.g = fg;	fore.b = fb;
	SDL_Color back;	back.r = br;	back.g = bg;	back.b = bb;

	SDL_Rect dest;	dest.x = x;		dest.y = y;		dest.h = dest.w = 0;

	SDL_Surface* temp = TTF_RenderText_Shaded( fonts[ font_index ], text, fore, back );

	SDL_BlitSurface( temp, NULL, screen, &dest );

	SDL_FreeSurface( temp );
}

void Sim::TextWriter::WriteText( SDL_Surface* screen, const char* font_file, int size, const char* text, int x, int y, int fr, int fg, int fb, int br, int bg, int bb )
{
	SDL_Color fore;		fore.r = fr;	fore.g = fg;	fore.b = fb;
	SDL_Color back;	back.r = br;	back.g = bg;	back.b = bb;

	SDL_Rect dest;		dest.x = x;		dest.y = y;		dest.w = dest.h = 0;

	TTF_Font* temp_font = TTF_OpenFont( font_file, size );

	SDL_Surface* temp = TTF_RenderText_Shaded( temp_font, text, fore, back );

	SDL_BlitSurface( temp, NULL, screen, &dest );

	SDL_FreeSurface( temp );

	TTF_CloseFont( temp_font );
}

void Sim::TextWriter::WriteText( SDL_Surface* screen, const char* font_file, int size, const char* text, int x, int y, int fr, int fg, int fb )
{
	SDL_Color fore;		fore.r = fr;	fore.g = fg;	fore.b = fb;

	SDL_Rect dest;		dest.x = x;		dest.y = y;		dest.w = dest.h = 0;

	TTF_Font* temp_font = TTF_OpenFont( font_file, size );

	SDL_Surface* temp = TTF_RenderText_Blended( temp_font, text, fore );

	SDL_BlitSurface( temp, NULL, screen, &dest );

	SDL_FreeSurface( temp );

	TTF_CloseFont( temp_font );
}
