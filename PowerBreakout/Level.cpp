// Implementation of Level	-	Copyright Ian Finlayson 2005

#include "Level.h"
#include <iostream>
#include <fstream>
#include <sstream>

Level::Level( )
{
	levelNumber = 0;
}

void Level::Load( int level )
{
	levelNumber = level;

	std::stringstream fileName;
	fileName << "PBdata/level" << level << ".bal";

	std::fstream file;
	file.open( fileName.str( ).c_str( ), std::ios::in );

	file >> ballSpeed;

	SDL_Color blue = { 0, 0, 255 };	//Used for Color Key

	for( int i=0; i<BLOCKS_PER_COLUM; i++ )
	{
		for( int j=0; j<BLOCKS_PER_ROW; j++ )
		{
			int colour;
			file >> colour;
			blocks[ i ][ j ].Load( BLOCK_WIDTH, BLOCK_HEIGHT, colour, (j*BLOCK_WIDTH) + WINDOW_BORDER, (i*BLOCK_HEIGHT) + WINDOW_BORDER, false );
		}
	}

	file.close( );
}

bool Level::LevelComplete( )
{
	for( int i=0; i<BLOCKS_PER_COLUM; i++ )
	{
		for( int j=0; j<BLOCKS_PER_ROW; j++ )
		{
			if( blocks[ i ][ j ].GetColor( ) != NODRAW )	//if on is not blank...
			{
				return( false );
			}
		}
	}

	return( true );
}

int Level::CollideWithBlocks( Entity* Ball )
{
	int deletions = 0;

	for( int i=0; i<BLOCKS_PER_COLUM; i++ )
	{
		for( int j=0; j<BLOCKS_PER_ROW; j++ )
		{
			if( blocks[ i ][ j ].GetColor( ) != NODRAW )	//If block is empty, can't collide
			{
				if( blocks[ i ][ j ].Collide( Ball->GetPosit( ) ) )	//If it collides
				{
					deletions++;
					RemoveBlock( i, j );

					//This is to fix a little problem. Without this next block of code,
					//If the ball hits a block from the side, it will bounce vertically
					//This results in the ball zooming across the level sideways taking out
					//A lot of blocks and looks very stupid. Remove and see if you don't beleive me.

					//Note that we only do this once no matter how many blocks the ball collides with
					//Also note that 3 is an offset - They can overlap a little and still count as horizontal
					//Not exactly pixel-perfect but hey who is?
					if( ( Ball->GetPosit().x + Ball->GetPosit().w ) < ( blocks[i][j].GetPosit().x - 3) )//If ball's right edge is left of blocks left edge
					{
						if( ( deletions == 1 ) && ( Ball->GetXSpeed() != 0 ) )
						{
							Ball->MultMovement( -1, -1 );	//We reverse both movements so that main reversing y will have no effect
						}
					}
					else if( ( Ball->GetPosit().x + 3 ) > ( blocks[i][j].GetPosit().x + blocks[i][j].GetPosit().w ) )
					{
						if( ( deletions == 1 ) && ( Ball->GetXSpeed() != 0 ) )
						{
							Ball->MultMovement( -1, -1 );
						}
					}
				}
			}
		}
	}

	return( deletions );
}

void Level::Draw( SDL_Surface* bitmap, SDL_Surface** screen )
{
	for( int i=0; i<BLOCKS_PER_COLUM; i++ )
	{
		for( int j=0; j<BLOCKS_PER_ROW; j++ )
		{
			blocks[ i ][ j ].Draw( bitmap, screen );
		}
	}
}

void Level::RemoveBlock( int i, int j )
{
	blocks[ i ][ j ].SetColor( NODRAW );
}

int Level::GetBallSpeed( )
{
	return( ballSpeed );
}