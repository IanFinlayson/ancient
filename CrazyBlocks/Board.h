// A Board is a 20x12 array of squares (row x column)	-	Copyright Ian Finlayson 2005

#ifndef BOARD_H
#define BOARD_H

#include "Tet.h"

class Board
{
private:
    Square squares[20][12];
    
public:
	Board();
	void Init();
	void Add( Tet newTet );
	void Draw( SDL_Surface** blocks, SDL_Surface** screen );
	bool Collide( Tet tet );
	bool Lost();
	int  DeletRows();	//returns the number of rows deleted
	void DeleteThis( int row_index );	//Does actual deletion
};

#endif


