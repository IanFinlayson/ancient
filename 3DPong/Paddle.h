// Paddle.h
// Ian Finlayson

#ifndef PADDLE_H
#define PADDLE_H

#include "Vector.h"

class Paddle
{
private:
	Vector<float, 3> position;
	Vector<float, 3> velocity;

	// Used for input
	bool KeyDown( char vk_code );

	bool KeyUp( char vk_code );

public:
	Paddle( );

	void Update( float elapsed_time );
	void Draw( );
	void Reset( );

	// Used by ball for collision testing...
	Vector<float, 3> GetPosition( );
};
#endif
