// The paddle, blocks, and ball are all		-	Copyright Ian Finlayson 2005
// Entities, ( I was going to use inheiritance,
// but tis turned out more logical - to me )

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL/SDL.h>

class Entity
{
private:
	int					color;			//Color of entity

	SDL_Rect			source;			//Source rect in Bitmap
	SDL_Rect			dest;			//Destination Rectangle

	float				 X,  Y;
	float				dX, dY;			//For fluid movement

	unsigned int start_time, end_time;
	
public:
	Entity( );
	Entity( int width, int height, int Color, int startX, int startY, bool IsBall );//Constructor
	~Entity();

	void Load( int width, int height, int Color, int startX, int startY, bool IsBall );

	void SetPosition( float X, float Y );
	void Move( float dX, float dY );
	void SetMovement( float dX, float dY );
	void MultMovement( float cX, float cY );
	void AddToMovement( float xP, float yP );
	void ReverseMovement( );

	float  GetXSpeed( );
	float  GetYSpeed( );
	SDL_Rect GetPosit( );
	int GetColor( );
	void SetColor( int Colour );

	void Draw( SDL_Surface* bitmap, SDL_Surface** screen );
	void Update( );

	bool Collide( SDL_Rect otherEntity );
	bool OutOfBounds( SDL_Rect playArea, bool totally );

	void AddToSrcRect( int xP, int yP );
};


#endif