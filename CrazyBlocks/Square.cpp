//Implements Square class	-	Copyright Ian Finlayson 2005

#include "Square.h"

Square::Square()		//Initialize to error values - Setters MUST be called
{
	dest.h = dest.w = dest.x = dest.y = 0;
	src.h  = src.w  = src.x  = src.y  = 0;

    color = -1;
}

int Square::GetColor()
{
	return( color );
}

void Square::Move( int dX, int dY )	// Works in pixels NOT subscripts
{
	dest.x = dest.x + dX;
	dest.y = dest.y + dY;
}

void Square::Draw( SDL_Surface** blocks, SDL_Surface** screen )
{
	//This Method is the WorkHorse of the whole program. It draws
	//Every part of the board and every part of the Tetramino
	//It is called 244 times every Frame...It is regrettable therefore,
	//that it also has to add 32 to the tets then subtract it back
	//This is horribly inefficieant (though + and - IS cpu cheap )
	//It is like this because I added a 32 pixel border and could not
	//Find a good way to modify the code to reflect this.
	//Oh well, I set the framerate to be 30 and my CPU keeps up fine
	dest.x += 32;
	dest.y += 32;

	SDL_BlitSurface( *blocks, &src, *screen, &dest );

	dest.y -= 32;
	dest.x -= 32;
}

void Square::SetColor( int Color )
{
	//Also must set the source rect in the .bmp
	color = Color;
	
	src.h = 16;
	src.w = 16;

	src.y = 0;
	src.x = (16*Color);
}

void Square::SetDest( SDL_Rect Dest )
{
	dest = Dest;
}