/********************************************************************
*	SimButton.cpp				 Ian Finlayson 2005					*
*																	*
*	Implementation of Sim::Button									*
********************************************************************/

#include <cstring>
#include "SimButton.h"
#include "SimException.h"

Sim::Button::Button( int x, int y, int w, int h, int borderSize, int border_r, int border_g, int border_b, int x_offset, int y_offset )
{
	position.x = x;		position.y = y;		position.w = w;		position.h = h;

	border_size = borderSize;

	border_color.r = border_r;	border_color.g = border_g;	border_color.b = border_b;

	x_off = x_offset;
	y_off = y_offset;

	is_pushed = false;
}

void Sim::Button::SetBackGround( SDL_Surface* image )
{
	if( image == NULL )
	{
		throw Sim::Exception( "Button::SetBackGround", "Image passed was NULL" );
	}

	back.image = image;

	back_is_image = true;
}

void Sim::Button::SetBackGround( int r, int g, int b )
{
	back.color = new SDL_Color;

	back.color->r = r;
	back.color->g = g;
	back.color->b = b;

	back_is_image = false;
}

void Sim::Button::SetForeGround( SDL_Surface* image )
{
	if( image == NULL )
	{
		throw Sim::Exception( "Button::SetForeGround", "Image passed was NULL" );
	}

	fore.image = image;

	fore_is_image = true;
}

void Sim::Button::SetForeGround( const char* text, TTF_Font* font, int fontR, int fontG, int fontB )
{
	if( font == NULL )
	{
		throw Sim::Exception( "Button::SetForeground", "Font passed was NULL" );
	}

	fore.text = new Text;

	fore.text->text = strdup(text);
	fore.text->font = font;

	fore.text->fontColor.r = fontR;
	fore.text->fontColor.g = fontG;
	fore.text->fontColor.b = fontB;

	fore_is_image = false;
}

void Sim::Button::HandleMouseDown( int mx, int my )
{
	if( ( mx > position.x ) && ( mx < (position.x + position.w) ) )	//if mouse is within horizontal area
	{
		if( ( my > position.y ) && ( my < (position.y + position.h) ) )	//if mouse is in vertically
		{
			is_pushed = true;
			return;
		}
	}

	is_pushed = false;
}

bool Sim::Button::HandleMouseUp( int mx, int my )
{
	if( ( mx > position.x ) && ( mx < (position.x + position.w ) ) )
	{
		if( ( my > position.y ) && ( my < (position.y + position.h) ) )
		{
			//Mouse is in button
			if( is_pushed )
			{
				is_pushed = false;
				return true;
			}
		}
	}

	is_pushed = false;
	return false;
}

void Sim::Button::Draw( SDL_Surface* screen )
{
	// First we draw the border...
	SDL_FillRect( screen, &position, SDL_MapRGB( screen->format, border_color.r, border_color.g, border_color.b ) );

	// Calulate a new position...
	SDL_Rect temp;

	temp.x = position.x + border_size;
	temp.y = position.y + border_size;
	temp.w = position.w - (2*border_size);
	temp.h = position.h - (2*border_size);

	if( is_pushed )	//if it's pushed, we draw it in the lower left.
	{
		temp.x += border_size;
		temp.y += border_size;
	}

	// Draw the background
	if( back_is_image )
	{
		SDL_BlitSurface( back.image, NULL, screen, &temp );
	}
	else
	{
		SDL_FillRect( screen, &temp, SDL_MapRGB( screen->format, back.color->r, back.color->g, back.color->b ) );
	}

	// Calculate a new position...
	temp.x += x_off;
	temp.y += y_off;

	if( fore_is_image )
	{
		//Assume user gave good sized image...
		temp.w = fore.image->w;
		temp.h = fore.image->h;

		SDL_BlitSurface( fore.image, NULL, screen, &temp );
	}
	else
	{
		SDL_Surface* textS;

		temp.h = temp.w = 0;

		if( back_is_image )	//mus use blended text
		{
			textS = TTF_RenderText_Blended( fore.text->font, fore.text->text, fore.text->fontColor );
		}
		else
		{
			textS = TTF_RenderText_Shaded( fore.text->font, fore.text->text, fore.text->fontColor, *back.color );
		}

		SDL_BlitSurface( textS, NULL, screen, &temp );

		SDL_FreeSurface( textS );
	}
}

void Sim::Button::ChangeButtonText( const char* text )
{
	fore.text->text =  strdup(text);
}

Sim::Button::~Button( )
{
	if( !back_is_image )
	{
		delete back.color;
	}

	if( !fore_is_image )
	{
		delete fore.text;
	}
}


