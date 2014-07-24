/********************************************************************
*	SimFunctions.cpp			 Ian Finlayson 2005					*
*																	*
*	Implementation of the disparate free functions in SimLib		*
********************************************************************/

#include "SimFunctions.h"

// Only a windows implementation of GetFiles exists right now :(
//#include <windows.h>
/*
std::vector< std::string > Sim::GetFiles( std::string directory )
{
	std::vector< std::string > files;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string dir = directory + "\\*";

	hFind = FindFirstFile( dir.c_str(), &FindFileData );

	if( hFind != INVALID_HANDLE_VALUE )
	{
		files.push_back( FindFileData.cFileName );

		while( FindNextFile( hFind, &FindFileData ) )
		{
			files.push_back( FindFileData.cFileName );
		}
	}
	
	return files;
}
*/
// Returns the direction from which a hits b
// NO COLLISION  -1
// EAST    0
// NORTH   1
// WEST    2
// SOUTH   3
// so if it returns 0, a is to the right of b

int Sim::Collide( SDL_Rect* a, SDL_Rect* b, int error )
{
	int lefA = a->x + error;
	int ritA = (a->x + a->w) - (2*error);
	int topA = a->y + error;
	int botA = (a->y + a->h) - (2*error);

	int lefB = b->x + error;
	int ritB = (b->x + b->w) - (2*error);
	int topB = b->y + error;
	int botB = (b->y + b->h) - (2*error);

	if( botA > topB )
		return 1;

	if( ritA > lefB )
		return 2;

	if( ritB > lefA )
		return 0;

	if( botB > topA )
		return 3;

	return -1;
}


