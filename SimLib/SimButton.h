/********************************************************************
*	SimButton.h					 Ian Finlayson 2005					*
*																	*
*	This class provides a simple way to do buttons with SDL			*
*	They can have an image or color for background and an image		*
*	or text for a foreground.										*
*																	*
********************************************************************/

#ifndef SIM_BUTTON
#define SIM_BUTTON

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

namespace Sim
{
	class Button
	{
	private:
		struct Text
		{
			char* text;
			TTF_Font* font;
			SDL_Color fontColor;
		};

		union Background
		{
			SDL_Surface* image;
			SDL_Color*	 color;
		};

		union Foreground
		{
			SDL_Surface* image;
			Text* text;
		};


		Background back;
		Foreground fore;

		bool back_is_image;
		bool fore_is_image;

		SDL_Rect position;
		int border_size;
		SDL_Color border_color;

		bool is_pushed;
		int x_off, y_off;

	public:
		//I considered setting all these in the constructors - but it was too many arguments!

		Button( int x, int y, int w, int h, int borderSize, int border_r, int border_g, int border_b, int x_offset, int y_offset );

		void SetBackGround( SDL_Surface* image );	//Call exactly one
		void SetBackGround( int r, int g, int b );	//of these two...

		void SetForeGround( SDL_Surface* image );											//...and exactly
		void SetForeGround( const char* text, TTF_Font* font, int fontR, int fontG, int fontB );	//one of these two


		void HandleMouseDown( int mx, int my );
		bool HandleMouseUp( int mx, int my );	// returns wether the user clicked in it

		void Draw( SDL_Surface* screen );

		void ChangeButtonText( const char* text );	// To change a "pause" to "unpause" for instance

		~Button( );
	};
}
#endif
