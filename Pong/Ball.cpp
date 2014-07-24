// Ball.cpp
// Ian Finlayson
// Implementation of the ball's methods...

#include <windows.h>
#include <GL/gl.h>

#include "Constants.h"
#include "Ball.h"

// Constructor
Ball::Ball( Paddle* paddle )
{
	// Seed rand with the system time for randomness
	srand( GetTickCount( ) );

	// Save a reference to the paddle. (We'll need it for collision tests)
	this->paddle = paddle;

	// Start with a random color...
	glColor3f( (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX );

	// Set up position and velocity
	Reset( );
}

// Returns whether or not the ball hits left wall
bool Ball::CollideWithLeftWall( )
{
	// If the left of ball is left of wall
	return ( (position[X] - BALL_RADIUS) < WALL_WIDTH );
}

// Returns whether or not the ball hits the top wall
bool Ball::CollideWithTopWall( )
{	
	// If the top of ball is above top wall
	return ( (position[Y] + BALL_RADIUS) > (WINDOW_HEIGHT - WALL_WIDTH) );
}

// Returns whether or not the ball hit the bottom
bool Ball::CollideWithBottomWall( )
{	
	// If the bottom of ball is past bottom wall
	return ( (position[Y] - BALL_RADIUS) < WALL_WIDTH );
}

// Returns whether the ball collided with the paddle
bool Ball::CollideWithPaddle( )
{
	// Get the paddles current position...
	RECT paddle_position = paddle->GetPosition( );

	// If the ball is below paddle...
	if( (position[Y] + BALL_RADIUS) < paddle_position.bottom )
	{
		return false;
	}
	// If the ball is above paddle
	if( (position[Y] - BALL_RADIUS) > paddle_position.top )
	{
		return false;
	}
	// If the ball is to the right of paddle
	if( (position[X] + BALL_RADIUS) < paddle_position.left )
	{
		return false;
	}

	// else it must have hit
	return true;
}

// Tests whether the ball has sailed off the screen...
bool Ball::OutOfBounds( )
{
	return ( position[X] > WINDOW_WIDTH );
}

// Returns whether the user has lost...
bool Ball::Update( float elapsed_time )
{
	// If the ball collides with the left wall
	if( CollideWithLeftWall( ) )
	{
		// Make sure X velocity is positive
		velocity[X] = abs( velocity[X] );

		// Set a random color
		glColor3f( (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX );
	}
	// If the ball collides with the Paddle
	else if( CollideWithPaddle( ) )
	{
		// Make sure the X velocity is negative
		velocity[X] = -abs( velocity[X] );

		// We calculate the balls position relative to the center of the paddle
		// in order to make the part of the paddle you hit the ball with affect
		// the movement of the ball.

		float local_y = position[Y] - ( paddle->GetPosition( ).bottom + (PADDLE_HEIGHT / 2) );

		// We increment the velocity by its relative position times some scale factor
		// the higher the scale factor is set, the more effect paddle position will have
		//// on the ball's resulting velocity...

		velocity[Y] = velocity[Y] + local_y*SPEED_FACTOR;

		// Now we make sure the speed is still the same...
		velocity.Normalize( );
		velocity = velocity*BALL_SPEED;

		// Set a random color
		glColor3f( (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX );
	}

	// If the ball collides with the top wall
	if( CollideWithTopWall( ) )
	{
		// Make sure Y velocity is negative
        velocity[Y] = -abs( velocity[Y] );

		// Set a random color
		glColor3f( (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX );
	}
	// if the ball collides with the bottom wall
	else if( CollideWithBottomWall( ) )
	{
		// Make sure Y velocity is positive
        velocity[Y] = abs( velocity[Y] );

		// Set a random color
		glColor3f( (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX, (float)rand( )/RAND_MAX );
	}

	// If user has lost
	if( OutOfBounds( ) )
	{
		return false;
	}

	// Update the balls position
	position = position + velocity*elapsed_time;

	return true;	// User has not lost...
}

void Ball::Draw( )
{
	// We draw the ball as a polygon
	glBegin( GL_POLYGON );

	// We loop through the circle by radians 0->2PI in increments of PI/16 (11.25 deg)
	for( float theta=0.0f; theta<2*PI; theta += (PI/16.0f) )
	{
		glVertex2f( position[X] + (BALL_RADIUS * cos(theta)), position[Y] + (BALL_RADIUS * sin(theta)) );
	}

	glEnd( );
}

void Ball::Reset( )
{
	// Reset position
	position[X] = BALL_START_X;
	position[Y] = BALL_START_Y;

	// Make the velocity random
	velocity[X] = (float)rand( );
	velocity[Y] = (float)rand( );

	// We do the following to prevent the intial velocity
	// from being all vertical (which would not be much fun)
	velocity[X] *= 10;	// Increse the ball's X component by a factor of 10


	// rand only returns positive numbers, but we want a chance of negative
	// initial velocity, so we do the following:
	if( rand( ) % 2 )
	{
		velocity[X] = -velocity[X];
	}
	if( rand( ) % 2 )
	{
		velocity[Y] = -velocity[Y];
	}

	// Normalize the velocity then multiply by speed so that the velocity vector
	// has random direction but the correct magnitude
	velocity.Normalize( );
	velocity = velocity*BALL_SPEED;
}
