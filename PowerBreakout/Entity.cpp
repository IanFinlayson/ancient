// Implementation of non-virtual	-	Copyright Ian Finlayson 2005
// Entity Methods

#include "Entity.h"
#include "Constants.h"

Entity::Entity( )//If this constructor is used, Load() must be called manually!
{

}

Entity::Entity( int width, int height, int Color, int startX, int startY, bool IsBall )
{
	Load( width, height, Color, startX, startY, IsBall );
}

void Entity::Load( int width, int height, int Color, int startX, int startY, bool IsBall )
{
	color = Color;

	source.h = height;
	source.w = width;
	source.y = 0;

	if( IsBall )
	{
		source.x = 0;
	}
	else
	{
		source.x = BLOCK_WIDTH * Color;
	}

	dest.h = height;
	dest.w = width;
	X = (float)startX;
	Y = (float)startY;

	dX = dY = 0;

	start_time = end_time = SDL_GetTicks( );

}

void Entity::Draw( SDL_Surface* bitmap, SDL_Surface** screen )
{
	dest.x = (int)X;
	dest.y = (int)Y;
	SDL_BlitSurface( bitmap, &source, *screen, &dest );
}

void Entity::Move( float dX, float dY )
{
	X += dX;
	Y += dY;
}

void Entity::SetPosition( float X, float Y)
{
	this->X = X;
	this->Y = Y;
}


void Entity::SetMovement( float dX, float dY )
{
	this->dX = dX;
	this->dY = dY;
}

void Entity::MultMovement( float cX, float cY )
{
	dX *= (int)cX;
	dY *= (int)cY;
}

void Entity::AddToMovement( float xP, float yP )
{
	dX += xP;
	dY += yP;
}

float Entity::GetXSpeed( )
{
	return( dX );
}

float Entity::GetYSpeed( )
{
	return( dY );
}

Entity::~Entity( )
{

}


void Entity::ReverseMovement( )
{
	dX *= -1.0f;
	dY *= -1.0f;
}

SDL_Rect Entity::GetPosit( )
{
	dest.x = (int)X;
	dest.y = (int)Y;

	return( dest );
}

int Entity::GetColor( )
{
	return( color );
}

void Entity::SetColor( int Colour )
{
	color = Colour;
	source.x = BLOCK_WIDTH * Colour;
}

void Entity::Update( )
{
	end_time = SDL_GetTicks( );

	unsigned int ms_since_last_frame = end_time - start_time;

	X += dX * (( static_cast<float>(ms_since_last_frame)) / 1000.0f);
	Y += dY * (( static_cast<float>(ms_since_last_frame)) / 1000.0f);

	start_time = end_time;
}

bool Entity::OutOfBounds( SDL_Rect playArea, bool totally )
{
	dest.x = (int)X;
	dest.y = (int)Y;

	if( totally )	//return whether player is fully out of area
	{
		if( ( dest.x + dest.w ) < playArea.x )	//player is out on left
		{
			return( true );
		}
		if( ( dest.x ) > ( playArea.x + playArea.w ) )	//player is out on right
		{
			return( true );
		}
		if( ( dest.y + dest.h ) < playArea.y )	//player is out on top
		{
			return( true );
		}
		if( (dest.y ) > ( playArea.y + playArea.h ) )
		{
			return( true );
		}
	}
	else			//return whether sprite is partially out of area
	{
		if( dest.x < playArea.x )	//out on left
		{
			return( true );
		}
		if( ( dest.x + dest.w ) > ( playArea.x + playArea.w ) )	//out on right
		{
			return( true );
		}
		if( dest.y < playArea.y )	//out on top
		{
			return( true );
		}
		if( ( dest.y + dest.h ) > ( playArea.y + playArea.h ) )	//out on bottom
		{
			return( true );
		}
	}

	return( false );
}

bool Entity::Collide( SDL_Rect otherEntity )
{	//Just Think about it  :) .........
	return( !OutOfBounds( otherEntity, true ) );
}

void Entity::AddToSrcRect( int xP, int yP )
{
	source.x += xP;
	source.y += yP;
}