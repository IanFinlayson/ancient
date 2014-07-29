// Paddle.cpp
// Ian Finlayson

#include <GLFW/glfw3.h>

#include "Constants.h"
#include "Paddle.h"

Paddle::Paddle( )
{
	// Just set the vectors up...
	Reset( );
}

void Paddle::Draw( )
{
	// Draw a blue line around edge...
	glLineWidth( 3.0f );
	glColor4f( 0.5f, 0.5f, 1.0f, 1.0f );
	glBegin( GL_LINE_LOOP );
		glVertex3f( -WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, position[Z] );
		glVertex3f( -WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, position[Z] );
		glVertex3f( WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, position[Z] );
		glVertex3f( WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, position[Z] );
	glEnd( );
	glLineWidth( 1.0f );

	// Draw the paddle....
	// A semi-transparent blue....
	glDepthMask( GL_FALSE );	// Turn off depth buffer writing...
	glColor4f( 0.5f, 0.5f, 1.0f, 0.6f );
	glBegin( GL_QUADS );
		glVertex3f( position[X] - PADDLE_WIDTH/2.0f, position[Y] + PADDLE_HEIGHT/2.0f, position[Z] );
		glVertex3f( position[X] - PADDLE_WIDTH/2.0f, position[Y] - PADDLE_HEIGHT/2.0f, position[Z] );
		glVertex3f( position[X] + PADDLE_WIDTH/2.0f, position[Y] - PADDLE_HEIGHT/2.0f, position[Z] );
		glVertex3f( position[X] + PADDLE_WIDTH/2.0f, position[Y] + PADDLE_HEIGHT/2.0f, position[Z] );
	glEnd( );
	glDepthMask( GL_TRUE );	// Turn it back on...
}

void Paddle::Reset( )
{
	// The default is 0...
	position[Z] = FRONT_Z;

}

void Paddle::Update( float elapsed_time )
{
	// Handle up/down
	if( KeyDown( VK_UP ) )
	{
		velocity[Y] = PADDLE_SPEED;
	}
	else if( KeyDown( VK_DOWN ) )
	{
		velocity[Y] = -PADDLE_SPEED;
	}
	else
	{
		velocity[Y] = 0.0f;
	}

	// Handle left/right
	if( KeyDown( VK_RIGHT ) )
	{
		velocity[X] = PADDLE_SPEED;
	}
	else if( KeyDown( VK_LEFT ) )
	{
		velocity[X] = -PADDLE_SPEED;
	}
	else
	{
		velocity[X] = 0.0f;
	}

	// Handle in/out
	if( KeyDown( 'O' ) )
	{
		velocity[Z] = -PADDLE_SPEED;
	}
	else if( KeyDown( 'L' ) )
	{
		velocity[Z] = PADDLE_SPEED;
	}
	else
	{
		velocity[Z] = 0.0f;
	}

	// Handle in/out collisions
	if( (position[Z] > FRONT_Z) && (velocity[Z] > 0.0f) )
	{
		velocity[Z] = 0.0f;
	}
	else if( (position[Z] < (BACK_Z+560.0f)) && (velocity[Z] < 0.0f) )
	{
		velocity[Z] = 0.0f;
	}

	// Handle left/right collisions
	if( ((position[X] - PADDLE_WIDTH/2.0f) < -WINDOW_WIDTH/2.0f) && (velocity[X] < 0.0f) )
	{
		velocity[X] = 0.0f;
	}
	else if( ((position[X] + PADDLE_WIDTH/2.0f) > WINDOW_WIDTH/2.0f) && (velocity[X] > 0.0f) ) 
	{
		velocity[X] = 0.0f;
	}

	// Handle up/down collisions
	if( ((position[Y] - PADDLE_HEIGHT/2.0f) < -WINDOW_HEIGHT/2.0f) && (velocity[Y] < 0.0f) )
	{
		velocity[Y] = 0.0f;
	}
	else if( ((position[Y] + PADDLE_HEIGHT/2.0f) > WINDOW_HEIGHT/2.0f) && (velocity[Y] > 0.0f) ) 
	{
		velocity[Y] = 0.0f;
	}

	// Update the vectors...
	position = position + velocity*elapsed_time;
}

Vector<float, 3> Paddle::GetPosition( )
{
	return position;
}
