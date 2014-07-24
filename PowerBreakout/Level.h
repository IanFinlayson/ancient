// Definition of a level	-	Copyright Ian Finlayson 2005
// A level consists of a grid
// of blocks.

/* In the file there is a number followed by a grid of 1's , 0's ie
12
1 2 3 2 1 2 3 2 1
4 0 0 0 0 0 0 0 1
5 0 0 0 0 0 0 0 1
4 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 1
4 0 0 0 0 0 0 0 1
5 0 0 0 0 0 0 0 1
4 0 0 0 0 0 0 0 1
1 2 3 2 1 2 3 2 1
Here 12 is the ball speed and the grid represents the level's blocks
The numbers 1 through 6 in the grid represent color - see Constants.h -

-Note- Ball speed should be like 1-5 where 1 is easy 5 is impossible!

*/

//This format is too trivial for a level editor!
//Level names are "data/level1.bal"

#ifndef LEVEL_H
#define	LEVEL_H

#include "Entity.h"
#include "Constants.h"

class Level
{
private:
	//addressing goes blocks[row num][col num]
	//(2-d arrays always mess me up.) Hey stop laughing!!

	int levelNumber;

	Entity blocks[ BLOCKS_PER_COLUM ][ BLOCKS_PER_ROW ];

	int ballSpeed;

public:
	Level( );

	void Draw( SDL_Surface* bitmap, SDL_Surface** screen );
	void RemoveBlock( int i, int j );

	void Load( int level );
	bool LevelComplete( );

	//Returns number of blocks deleted
	int CollideWithBlocks( Entity* Ball );

	int GetBallSpeed( );

};


#endif