// Implement Tet class	-	Copyright Ian Finlayson 2005

#include "Tet.h"

Tet::Tet()	// Initializes to error codes - MUST call (Re)Init
{
	type = -1;
	centerX = centerY = -1;
}

int Tet::GetType()
{
	return( type );
}

void Tet::ReInit( int Type )
{
	type = Type;
	SDL_Rect destined;	//Destination rect
	destined.h = destined.w = 16;	// Always 16x16
	
	for( int i=0; i<4; i++ )	//Set colors/source RECT
	{
		squares[i].SetColor( type );
	}

	if( type == RED )
	{
		destined.y = 0;

		destined.x = 64;
		squares[0].SetDest( destined );

		destined.x = 80;
		squares[1].SetDest( destined );

		destined.x = 96;
		squares[2].SetDest( destined );

		destined.x = 112;
		squares[3].SetDest( destined );

		centerX = 96;
		centerY = 16;
	}
	else if( type == BLUE )
	{
		destined.y = 0;

		destined.x = 80;
		squares[0].SetDest( destined );

		destined.x = 96;
		squares[1].SetDest( destined );

		destined.y = 16;

		destined.x = 80;
		squares[2].SetDest( destined );

		destined.x = 96;
		squares[3].SetDest( destined );

		centerX = 96;
		centerY = 16;
	}
	else if( type == YELLOW )
	{
		destined.y = 0;
		destined.x = 80;
		squares[0].SetDest( destined );

		destined.y = 16;
		destined.x = 64;
		squares[1].SetDest( destined );

		destined.x = 80;
		squares[2].SetDest( destined );

		destined.x = 96;
		squares[3].SetDest( destined );

		centerX = 96;
		centerY = 16;
	}
	else if( type == GREEN )
	{
		destined.y = 0;
		destined.x = 64;
		squares[0].SetDest( destined );

		destined.x = 80;
		squares[1].SetDest( destined );

		destined.y = 16;
		destined.x = 80;
		squares[2].SetDest( destined );

		destined.x = 96;
		squares[3].SetDest( destined );

		centerX = 96;
		centerY = 16;
	}
	else if( type == ORANGE )
	{
		destined.y = 0;
		destined.x = 80;
		squares[0].SetDest( destined );

		destined.x = 96;
		squares[1].SetDest( destined );

		destined.y = 16;
		destined.x = 64;
		squares[2].SetDest( destined );

		destined.x = 80;
		squares[3].SetDest( destined );

		centerX = 80;
		centerY = 16;
	}
	else if( type == PURPLE )
	{
		destined.y = 0;
		destined.x = 96;
		squares[0].SetDest( destined );

		destined.y = 16;
		destined.x = 64;
		squares[1].SetDest( destined );

		destined.x = 80;
		squares[2].SetDest( destined );

		destined.x = 96;
		squares[3].SetDest( destined );

		centerY = 16;
		centerX = 96;
	}
	else if( type == GREY )
	{
		destined.y = 0;
		destined.x = 64;
		squares[0].SetDest( destined );

		destined.x = 80;
		squares[1].SetDest( destined );

		destined.x = 96;
		squares[2].SetDest( destined );

		destined.y = 16;
		squares[3].SetDest( destined );

		centerX = 96;
		centerY = 16;
	}
}

void Tet::Move( int dX, int dY )
{
	centerX += dX;
	centerY += dY;

	for( int i=0; i<4; i++ )
	{
		squares[i].Move( dX, dY );
	}
}

void Tet::Draw( SDL_Surface** blocks, SDL_Surface** screen )
{
	for( int i=0; i<4; i++ )
	{
		squares[i].Draw( blocks, screen );
	}
}

//Rotate only works for 90 and 270. I could make it work for any angle,
//But that would require calls to sin() and cos(). Using integers,
//Only when theta is divisible by 90 will the results be valid anyway
void Tet::Rotate( int theta )
{
	if( type == BLUE )	//We dont wanna rotate blues
	{
		return;
	}
	SDL_Rect newRect;
	newRect.h = newRect.w = 16;

	for( int i=0; i<4; i++ )		//Move the tets to origin
	{
		squares[i].Move( -centerX, -centerY );
	}

	if( theta == 90 )
	{
		for( int i=0; i<4; i++ )
		{
			newRect.x = ( squares[i].dest.y * -1 );
			newRect.y = ( squares[i].dest.x );

			squares[i].SetDest( newRect );
		}	
	}
	else if( theta == 270 )
	{
		for( int i=0; i<4; i++ )
		{
			newRect.x = ( squares[i].dest.y );
			newRect.y = ( squares[i].dest.x * -1 );

			squares[i].SetDest( newRect );
		}
	}
	else
	{
		//Only does 90, 270
	}

	for( int i=0; i<4; i++ )
	{
		squares[i].Move( centerX, centerY );
	}
}

bool Tet::CollideWalls()
{
	for( int i=0; i<4; i++ )
	{
		if( ( squares[i].dest.x < 0 ) || ( squares[i].dest.x > 176 ) )	//horizontal collision
		{
			return( true );
		}
		if( ( squares[i].dest.y < 0 ) || ( squares[i].dest.y > 304 ) )	//vertical collision
		{
			return( true );
		}
	}

	return( false );
}