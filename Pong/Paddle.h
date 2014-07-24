// Paddle.h
// Ian Finlayson
// Defines the Paddle

#ifndef PADDLE_H
#define PADDLE_H

#include "Vector.h"

class Paddle
{
private:
	Vector<float, 2> position;	// Position and velocity
	Vector<float, 2> velocity;	// stored in vectors...

	// Used for input
	inline bool KeyDown( char vk_code )
	{
		return ( (GetAsyncKeyState( vk_code ) & 0x8000)? true : false );
	}

	inline bool KeyUp( char vk_code )
	{
		return !KeyDown( vk_code );
	}

public:
	Paddle( );

	void Update( float elapsed_time );
	void Draw( );

	// Returns the current position, (To the Ball)
	RECT GetPosition( );
};
#endif
