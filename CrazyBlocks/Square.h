// A square is one little thingy	-	Copyright Ian Finlayson 2005
//The tetraminoes and the board are composed of
//Squares. this makes it MUCH easier to implement the drawing code

#ifndef SQUARE_H
#define SQUARE_H

#include <SDL/SDL.h>
#include "Constants.h"

class Square
{
private:
	SDL_Rect src;
	int color;			//One from defines

public:
	SDL_Rect dest;
	Square();
	void Move( int dx, int dy );
	void Draw( SDL_Surface** blocks, SDL_Surface** screen );
	void SetColor( int Color );		//Also adjusts src rect
	void SetDest( SDL_Rect Dest );
	int  GetColor();
};

#endif