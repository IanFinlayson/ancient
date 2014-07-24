/********************************************************************
*	SimFunctions.h				 Ian Finlayson 2006					*
*																	*
*	These are just a bunch of unrelated function prototypes			*
*	They are here because none of them really warant their own file *
********************************************************************/

#ifndef SIM_FUNCTIONS
#define SIM_FUNCTIONS

#include <vector>
#include <string>
#include <SDL/SDL.h>

namespace Sim
{
	// For now there is only a windows implementation
	//std::vector< std::string > GetFiles( std::string directory );
	
	// Bounding box collision detections...
	// Returns the direction from which a hits b
	// NO COLLISION  -1
	// EAST    0
	// NORTH   1
	// WEST    2
	// SOUTH   3
	// so if it returns 0, a is to the right of b
	int Collide( SDL_Rect* a, SDL_Rect* b, int error=0 );

}

#endif
