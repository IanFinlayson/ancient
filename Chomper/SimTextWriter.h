/********************************************************************
*	SimTextWriter.h				 Ian Finlayson 2005					*
*																	*
*	This class provides an easy way to write text on screen			*
*																	*
********************************************************************/

#ifndef SIM_TEXTWRITER
#define SIM_TEXTWRITER

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

namespace Sim
{
	class TextWriter
	{
	private:
		TTF_Font** fonts;	//array of fonts
		int num_fonts;
		int curr_font;

	public:
		TextWriter( int numFonts );
		~TextWriter( );

		int AddFont( const char* font_name, int size );	// Returns the array index number

		TTF_Font* GetFont( int font_index );

		void WriteText( SDL_Surface* screen, int font_index, const char* text, int x, int y, int fr, int fg, int fb );	// Local font with blending
		void WriteText( SDL_Surface* screen, int font_index, const char* text, int x, int y, int fr, int fg, int fb, int br, int bg, int bb );	// Local font with plain background

		static void WriteText( SDL_Surface* screen, const char* font_file, int size, const char* text, int x, int y, int fr, int fg, int fb );	// Unloaded font with blending
		static void WriteText( SDL_Surface* screen, const char* font_file, int size, const char* text, int x, int y, int fr, int fg, int fb, int br, int bg, int bb );	// Unloaded font with plain background
	};
}
#endif
