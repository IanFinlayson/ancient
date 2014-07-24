/********************************************************************
*	SimTextField.h				 Ian Finlayson 2005					*
*																	*
*	This class provides a way for users to enter text on screen		*
*																	*
********************************************************************/

#ifndef SIM_TEXTFIELD
#define SIM_TEXTFIELD

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>

namespace Sim
{
	class TextField
	{
	private:
		static const int FLASH_INTERVAL = 750;	//ms between cursor flashes

		SDL_Rect position;	// on-screen position

		TTF_Font* font;		// font used

		std::string text1;	// text seen is text1|text2
		std::string text2;	// | is the cursor thingy

		unsigned int max_length;	// keeps from over-writing boundries

		unsigned int timer;	// keeps track of flashing cursor
		bool	on;		// whether cursor is appearing

		SDL_Color back_color;
		SDL_Color text_color;
		SDL_Color bord_color;

		int border_size;

		int x_off, y_off;	// offset of text

		bool almost_active;		// keeps track of whether
		bool active;			// the field has 'focus'

		bool set_repeat;

	public:
		// Longest constructor in the history of programming!

		TextField( int x, int y, int w, int h, int borderSize, int bord_r, int bord_g, int bord_b, TTF_Font* Font, 
			int maxLength, int back_r, int back_g, int back_b, int text_r, int text_g, int text_b, int x_offset,
			int y_offset, const char* start_text, bool key_repeat );

		void HandleMouseDown( int mX, int mY );
		void HandleMouseUp( int mX, int mY );

		bool HandleKeyDown( SDL_keysym keysym );	//returns whether the user hits enter

		void Draw( SDL_Surface* );

		std::string GetText( )
		{
			return ( text1 + text2 );	// the beauty of overloaded operators!
		}

		bool IsActive( )
		{
			return active;
		}

		~TextField( );
	};
}
#endif
