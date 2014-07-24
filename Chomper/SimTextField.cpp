/********************************************************************
*	SimTextField.cpp			 Ian Finlayson 2005					*
*																	*
*	Implements Sim::TextField										*
*																	*
********************************************************************/
#include <string>
#include "SimTextField.h"

Sim::TextField::TextField( int x, int y, int w, int h, int borderSize, int bord_r, int bord_g, int bord_b,
						  TTF_Font* Font, int maxLength, int back_r, int back_g, int back_b, int text_r, int text_g,
						  int text_b, int x_offset, int y_offset, const char* start_text, bool key_repeat )
{
	position.x = x;		position.y = y;		position.w = w;		position.h = h;

	border_size = borderSize;

	bord_color.r = bord_r;		bord_color.g = bord_g;		bord_color.b = bord_b;

	font = Font;
	max_length = maxLength;

	back_color.r = back_r;		back_color.g = back_g;		back_color.b = back_b;
	text_color.r = text_r;		text_color.g = text_g;		text_color.b = text_b;

	x_off = x_offset;	y_off = y_offset;

	text1 = start_text;
	text2 = "";

	timer = SDL_GetTicks( );
	on = true;

	almost_active = false;
	active = false;

	if( key_repeat )
	{
		// It's *very* annoying if this is not set!
		SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL );

		set_repeat = true;
	}
	else
	{
		set_repeat = false;
	}
}

Sim::TextField::~TextField( )
{
	if( set_repeat )
	{
		SDL_EnableKeyRepeat( 0, 0 );
	}
}

void Sim::TextField::HandleMouseDown( int mX, int mY )
{
	//We don't want to do anyting if the mouse is not in the textfield area...
	if( ( mX < position.x ) || ( mX > (position.x + position.w) ) )	//if mouse is not in the area horizontally
	{
		active = false;
		almost_active = false;
	}
	if( ( mY < position.y ) || ( mY > (position.y + position.h) ) ) //if mouse is not in the area vertically
	{
		active = false;
		almost_active = false;
	}

	//This means the user clicked down in the area...
	almost_active = true;
}

void Sim::TextField::HandleMouseUp( int mX, int mY )
{
	//Again, we don't care if the mouse is not in the textField's area
	if( ( mX < position.x ) || ( mX > (position.x + position.w) ) )	//if mouse is not in the area horizontally
	{
		almost_active = false;
		active = false;
	}
	if( ( mY < position.y ) || ( mY > (position.y + position.h) ) ) //if mouse is not in the area vertically
	{
		almost_active = false;
		active = false;
	}	

	if( almost_active )
	{
		almost_active = false;
		active = true;
	}
}

// This fuction is the whole crux of the operation really...
bool Sim::TextField::HandleKeyDown( SDL_keysym keysym )
{
	//If the field does not have focus, we don't want junk showing up...
	if( !active )
	{
		return false;
	}
	
	char input;

	//If user hits Enter - we let the calling code know...
	if( keysym.sym == SDLK_RETURN )
	{
		return true;
	}

	//If user hits backspace, we must delete last key hit...
	if( keysym.sym == SDLK_BACKSPACE )
	{
		if( !text1.empty() )
		{
			char* temp = (char*)text1.c_str();	//temp is strings data...
		
			temp[text1.size()-1] = '\0';		//move the NULL back one...

			text1 = temp;						//re-assign our string
		}										//Kind of hackish I know ;)
	}

	//If they Hit left or right, we need to adjust the strings...
	else if( keysym.sym == SDLK_LEFT )
	{
		if( !text1.empty() )
		{
			char* temp = (char*)text1.c_str( );		//Temp points to text1

			char last = temp[ text1.length() - 1 ];	//Get the last char

			temp[ text1.length() - 1 ] = '\0';		//Change it to NULL
	
			text1 = temp;							//Assign it to text1

			text2.insert( 0, 1, last );				//Put the last char to front of text2
		}
	}
	else if( keysym.sym == SDLK_RIGHT )
	{
		if( !text2.empty() )
		{
			char* temp = (char*)text2.c_str( );			//Temp points to text2

			char first = temp[ 0 ];					//Save first char

			temp++;									//Add 1 to 'delete' it

			text2 = temp;		//Reassign it to altered value...
			
			text1.append( 1, first );	//Append the char to text1
		}
	}

	//If user types more than allowed...
	else if( (text1.size() + text2.size()) >= max_length )
	{
		return( false );
	}


	//Next we test if the key was a letter...
	else if( (keysym.sym >= SDLK_a) && (keysym.sym <= SDLK_z ) )
	{
		input = keysym.sym;

		//If they had shift or caps on, capitalize it!
		if( keysym.mod & KMOD_CAPS )
		{
			//Capitaize It!
			input = ((char)keysym.sym - 32);
		}	
		else if( keysym.mod & KMOD_LSHIFT )
		{
			//Capitaize It!
			input = ((char)keysym.sym - 32);
		}
		else if( keysym.mod & KMOD_RSHIFT )
		{
			//Capitaize It!
			input = ((char)keysym.sym - 32);
		}

		//Add letter to the field...
		text1.push_back( input );
	}
	// Or if it's a space or a number...
	else if( (keysym.sym == SDLK_SPACE) || ((keysym.sym >= SDLK_0) && (keysym.sym <= SDLK_9)) )
	{
		//We add it...
		text1.push_back( keysym.sym );
	}
	//We also allow underscores if spaces not appropriate...
	else if( (keysym.sym == SDLK_UNDERSCORE) || (keysym.sym == SDLK_MINUS) )
	{
		text1.push_back( '_' );
	}

	return false;
}

void Sim::TextField::Draw( SDL_Surface* screen )
{
	//First, we draw the border...
	SDL_FillRect( screen, &position, SDL_MapRGB( screen->format, bord_color.r, bord_color.g, bord_color.b ) );

	//Next, we draw the background...
	SDL_Rect backDest;
	backDest.x = position.x + border_size;
	backDest.y = position.y + border_size;
	backDest.w = position.w - border_size*2;
	backDest.h = position.h - border_size*2;

	SDL_FillRect( screen, &backDest, SDL_MapRGB( screen->format, back_color.r, back_color.g, back_color.b ) );

	//Next we draw the string...

	std::string outText;

	if( active )
	{
		if( (SDL_GetTicks() - timer) > FLASH_INTERVAL )
		{
			on = !on;

			timer = SDL_GetTicks( );
		}

		if( on )
		{
			outText = text1 + "|";
			outText = outText + text2;
		}
		else
		{
			outText = text1 + " ";
			outText = outText + text2;
		}
	}
	else
	{
		outText = text1 + text2;
	}

	SDL_Surface* temp = TTF_RenderText_Shaded( font, outText.c_str(), text_color, back_color );

	SDL_Rect textPlace;
	textPlace.x = backDest.x + x_off;
	textPlace.y = backDest.y + y_off;
	textPlace.h = 0;
	textPlace.w = 0;

	SDL_BlitSurface( temp, NULL, screen, &textPlace );

	//And clean up after ourselves...
	SDL_FreeSurface( temp );
}
