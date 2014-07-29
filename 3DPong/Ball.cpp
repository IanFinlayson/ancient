// Ball.cpp
// Ian Finlayson

#include <GLFW/glfw3.h>

#include "Constants.h"
#include "Ball.h"
#include "Paddle.h"

Ball::Ball( Paddle* paddle )
{
	// Create the quadric...
	sphere = gluNewQuadric( );
	gluQuadricDrawStyle( sphere, GLU_FILL );

	// Seed the randomizer...
	srand( GetTickCount( ) );
	Reset( );

	// Stroe a pointer to the paddle
	this->paddle = paddle;
}

Ball::~Ball( )
{
	gluDeleteQuadric( sphere );
}

void Ball::Reset( )
{
	position[X] = 0.0f;
	position[Y] = 0.0f;
	position[Z] = BACK_Z;

	velocity[X] = static_cast<float>( rand( ) );
	velocity[Y] = static_cast<float>( rand( ) );
	velocity[Z] = RAND_MAX;	// This is the max so that it does not become too low relative to the
							// others. Once normalized it will make velocity[Z] ~ 0 Not Fun!

	// Force it's magnitude to be ball speed
	velocity.Normalize( );
	velocity = velocity*BALL_SPEED;
}

bool Ball::Update( float elapsed_time )
{
	// Check front/back wall collisions
	if( (position[Z] - BALL_RADIUS) < BACK_Z )
	{
		velocity[Z] = abs( velocity[Z] );
	}
	// The 250 is so the ball is much past the back...
	else if( position[Z] > (FRONT_Z+250) )
	{
		losses++;	// Update the loss count
		return false;
	}

	// Check left/right collisions
	if( (position[X] - BALL_RADIUS) < -WINDOW_WIDTH/2.0f )
	{
		velocity[X] = abs( velocity[X] );
	}
	else if( (position[X] + BALL_RADIUS) > WINDOW_WIDTH/2.0f )
	{
		velocity[X] = -abs( velocity[X] );
	}

	// Check top/bottom collisions
	if( (position[Y] - BALL_RADIUS) < -WINDOW_HEIGHT/2.0f )
	{
		velocity[Y] = abs( velocity[Y] );
	}
	else if( (position[Y] + BALL_RADIUS) > WINDOW_HEIGHT/2.0f )
	{
		velocity[Y] = -abs( velocity[Y] );
	}

	// Check paddle collision
	Vector<float, 3> paddle_pos = paddle->GetPosition( );
	if( ((position[Z] + BALL_RADIUS) >= paddle_pos[Z]) && ((position[Z] - BALL_RADIUS) <= paddle_pos[Z]) )
	{
		if( abs(position[X] - paddle_pos[X]) < max(BALL_RADIUS, PADDLE_WIDTH/2.0f) )
		{
			if( abs(position[Y] - paddle_pos[Y]) < max(BALL_RADIUS, PADDLE_HEIGHT/2.0f) )
			{
				// Paddle - Ball Collision!
				if( position[Z] < paddle_pos[Z] )	// paddle hit ball in proper direction
				{
					velocity[Z] = -abs( velocity[Z] );
					bounces++;
				}
				else	// paddle hit ball backwars (stupid)
				{
					velocity[Z] = abs( velocity[Z] );
				}
			}
		}
	}

	position = position + velocity*elapsed_time;

	// Not lost yet...
	return true;
}

void Ball::Draw( )
{
	// Draw the sphere at the position...
	glColor4f( 0.75f, 0.25f, 0.25f, 1.0f );

	glPushMatrix( );
	glTranslatef( position[X], position[Y], position[Z] );
	gluSphere( sphere, BALL_RADIUS, 32, 32 );
	glPopMatrix( );

	// Draw a rectangle for it's position on sides...
	//Make the line larger...
	glLineWidth( 3.0f );
	glBegin( GL_LINE_LOOP );
		glVertex3f( -WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, position[Z] );
		glVertex3f( -WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, position[Z] );
		glVertex3f( WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, position[Z] );
		glVertex3f( WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, position[Z] );
	glEnd( );
	glLineWidth( 1.0f );
}



