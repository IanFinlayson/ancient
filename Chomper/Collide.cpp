//Defines the Coolision function (Will be put in SimLib soon)

#include <SDL/SDL.h>
#include "Definitions.h"

bool Chomp::Collide( SDL_Rect* a, SDL_Rect* b )
{
	int error = 4;	// measured in pixels

	int lefA = a->x + error;
	int ritA = (a->x + 32) - (2*error);
	int topA = a->y + error;
	int botA = (a->y + 32) - (2*error);

	int lefB = b->x + error;
	int ritB = (b->x + 32) - (2*error);
	int topB = b->y + error;
	int botB = (b->y + 32) - (2*error);

	if( ( ritA <= lefB ) || ( ritB <= lefA ) )
	{
		return false;
	}

	if( ( botA <= topB ) || ( botB <= topA ) )
	{
		return false;
	}

	return true;
}