// Implements Board class	-	Copyright Ian Finlayson 2005

#include "Board.h"

Board::Board()		//Initializes Board to be all blanks - and positions
{
	Init();
}

void Board::Init()
{
	for( int i=0; i<20; i++ )
	{
		for( int j=0; j<12; j++ )
		{
			SDL_Rect dest;

			dest.h = 16;
			dest.w = 16;
			dest.x = j*16;
			dest.y = i*16;

			squares[i][j].SetColor( NODRAW );
			squares[i][j].SetDest( dest );
		}
	}
}

bool Board::Lost()
{
	for( int i=0; i<12; i++ )
	{
		if( squares[0][i].GetColor() != NODRAW )
		{
			return( true );
		}
	}

	return( false );
}

void Board::Add( Tet newTet )
{
	for( int i=0; i<4; i++ )
	{
		squares[newTet.squares[i].dest.y / 16][newTet.squares[i].dest.x / 16].SetColor( newTet.GetType() );
	}
}

void Board::Draw( SDL_Surface** blocks, SDL_Surface** screen )
{
	for( int i=0; i<20; i++ )
	{
		for( int j=0; j<12; j++ )
		{
			squares[i][j].Draw( blocks, screen );
		}
	}
}

bool Board::Collide( Tet tet )
{
	for( int i=0; i<4; i++ )
	{
		if( squares[ tet.squares[i].dest.y / 16][tet.squares[i].dest.x / 16].GetColor() != NODRAW )
		{
			return( true );
		}
	}

	return( false );
}

int Board::DeletRows()
{
	int deletions = 0;
	bool deleteMe = false;

	for( int i=0; i<20; i++ )	//for each row
	{
		deleteMe = true;		//Assume we will delete it
		
		for( int j=0; j<12; j++ )	//for each column
		{
			if( squares[i][j].GetColor() == NODRAW )	//if it's blank,
			{
				deleteMe = false;			//we won't delete it
			}

		}
		
		if( deleteMe )
		{
			deletions++;
			DeleteThis( i );		//delete this column
		}

	}

	return( deletions );
}

void Board::DeleteThis( int row_index )	//May Have Serious Errors
{
	for( int i=0; i<12; i++ )	//for each column
	{
		squares[ row_index ][ i ].SetColor( NODRAW );	//Blanks out current row
	}

	for( int i=(row_index-1); i>=0; i-- )	//for each row above index
	{
		for( int j=0; j<12; j++ )	//for each column
		{
			squares[i+1][j].SetColor( squares[i][j].GetColor() );	//make bottom = top
		}
	}
}



