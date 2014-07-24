// Ball.h
// Ian Finlayson
// Defines the Ball

#ifndef BALL_H
#define BALL_H

#include "Vector.h"
#include "Paddle.h"

class Ball
{
private:
	Vector<float, 2> position;		// Current Position vector
	Vector<float, 2> velocity;		// Current Velocity vector

	// A pointer to the paddle (for collision checks)
	Paddle* paddle;

	// Private collision testers
	bool CollideWithLeftWall( );
	bool CollideWithTopWall( );
	bool CollideWithBottomWall( );
	bool CollideWithPaddle( );
	bool OutOfBounds( );

public:
	Ball( Paddle* paddle );

	// Update returns a "lost" condition
	bool Update( float elapsed_time );
	void Reset( );
	void Draw( );
};

#endif