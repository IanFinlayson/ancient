// A tetramino is 4 squares arranged	-	Copyright Ian Finlayson 2005
// There is only one tetramino in the main code
// When the user loses, it is moved to the top
// A new one is not created

#ifndef TET_H
#define TET_H

#include "Square.h"

class Tet
{
private:
	int type;
	int centerX, centerY;

public:
	Square squares[4];
	Tet();
	void ReInit( int Type );
	void Move( int dX, int dY );
	void Rotate( int theta );
	void Draw( SDL_Surface** blocks, SDL_Surface** screen );
	bool CollideWalls();
	int	 GetType();
};

#endif