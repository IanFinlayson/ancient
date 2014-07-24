// Paddle.cpp
// Ian Finlayson
// Implements the Paddle

#include <windows.h>
#include <GL/gl.h>

#include "Constants.h"
#include "Paddle.h"

// Constructor
Paddle::Paddle( )
{
	position[X] = PADDLE_X;
	position[Y] = PADDLE_Y_START;

	velocity[X] = 0.0f;
	velocity[Y] = 0.0f;
}

void Paddle::Update( float elapsed_time )
{
	// If user moves up
	if( KeyDown( VK_UP ) )
	{
		// Prevent them from going off screen
		if( !((position[Y] + (PADDLE_HEIGHT / 2)) > (WINDOW_HEIGHT - WALL_WIDTH) ) )
		{
			velocity[Y] = PADDLE_SPEED;
		}
		else
		{
			velocity[Y] = 0.0f;
		}
	}
	// If user moves down
	else if( KeyDown( VK_DOWN ) )
	{
		// Prevent them from going off screen
		if( !((position[Y] - (PADDLE_HEIGHT / 2)) < WALL_WIDTH ) )
		{
			velocity[Y] = -PADDLE_SPEED;
		}
		else
		{
			velocity[Y] = 0.0f;
		}
	}
	// If neither key is pressed
	else if( KeyUp( VK_UP ) && KeyUp( VK_DOWN ) )
	{
		velocity[Y] = 0.0f;
	}

	// Update the position Vector
	position = position + velocity*elapsed_time;
}

void Paddle::Draw( )
{
	// Draw the rectangular part...
	glBegin( GL_QUADS );
		glVertex2f( position[X] - (PADDLE_WIDTH / 2.0f) + 1, position[Y] + ((PADDLE_HEIGHT-PADDLE_WIDTH) / 2.0f) );
		glVertex2f( position[X] - (PADDLE_WIDTH / 2.0f) + 1, position[Y] - ((PADDLE_HEIGHT-PADDLE_WIDTH) / 2.0f) );
		glVertex2f( position[X] + (PADDLE_WIDTH / 2.0f), position[Y] - ((PADDLE_HEIGHT-PADDLE_WIDTH) / 2.0f) );
		glVertex2f( position[X] + (PADDLE_WIDTH / 2.0f), position[Y] + ((PADDLE_HEIGHT-PADDLE_WIDTH) / 2.0f) );
	glEnd( );

	// The code to draw a circle basically loops from 0 to 2PI and defines x as
	// center[X] + radius*cos(theta)		and y as	center[Y] + radius*sin(theta)

	// Draw the top circle...
	glBegin( GL_POLYGON );
	for( float theta=0.0f; theta<=(2.0f*PI); theta += (PI/16.0f) )
	{
		glVertex2f( position[X] + (PADDLE_WIDTH / 2.0f)*cos(theta), (position[Y] + (PADDLE_HEIGHT/2.0f) - (PADDLE_WIDTH / 2.0f) ) + (PADDLE_WIDTH / 2.0f)*sin(theta) );
	}
	glEnd( );

	// Draw the bottom circle...
	glBegin( GL_POLYGON );
	for( float theta=0.0f; theta<=(2.0f*PI); theta += (PI/16.0f) )
	{
		glVertex2f( position[X] + (PADDLE_WIDTH / 2.0f)*cos(theta), (position[Y] - (PADDLE_HEIGHT/2.0f) + (PADDLE_WIDTH / 2.0f) ) + (PADDLE_WIDTH / 2.0f)*sin(theta) );
	}
	glEnd( );

}

// Returns the current position of the paddle (So the ball can check for collisions)
RECT Paddle::GetPosition( )
{
	RECT posit;

	posit.bottom = (int)position[Y] - (PADDLE_HEIGHT / 2);
	posit.top = (int)position[Y] + (PADDLE_HEIGHT / 2);
	posit.left = (int)position[X] - (PADDLE_WIDTH / 2);
	posit.right = (int)position[X] + (PADDLE_WIDTH / 2);

	return posit;
}


