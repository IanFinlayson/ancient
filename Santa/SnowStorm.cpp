// SnowStorm.cpp
// Ian Finlayson

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Constants.h"
#include "SnowStorm.h"

SnowStorm::SnowStorm( )
{
	// Initial vectors
	Vector<float, 2> sp;
	Vector<float, 2> sv;

	// Seed the randomizer with the curren time
	srand( GetTickCount( ) );

	// Set up some default values...
	for( int i=0; i<MAX_FLAKES; i++ )
	{
		sp[X] = ((float)rand( ) / (float)RAND_MAX) * 8.0f;

		// We want ~half to be to the left of the origin...
		if( rand( ) % 2 )
			sp[X] = -sp[X];

		sp[Y] = ((float)rand( ) / (float)RAND_MAX) * 8.0f;

		sv[X] = ((float)rand( ) / (float)RAND_MAX) * 0.5f;
		sv[Y] = ((float)rand( ) / (float)RAND_MAX) - 2.0f;

		flakes[i].position = sp;
		flakes[i].velocity = sv;
	}

	// Big snow flakes...
	glPointSize( 3.0f );
}

void SnowStorm::Draw( )
{
	// Clear out the view matrix...
	glLoadIdentity( );

	// We want them pure white, not textured...
	glDisable( GL_TEXTURE_2D );

	// White points...
	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_POINTS );
		for( int i=0; i<MAX_FLAKES; i++ )
			glVertex3f( flakes[i].position[X], flakes[i].position[Y], -10.0f );
	glEnd( );

	glEnable( GL_TEXTURE_2D );
}

void SnowStorm::Update( float elapsed_time )
{
	for( int i=0; i<MAX_FLAKES; i++ )
	{
		// Update position
		flakes[i].position = flakes[i].position + flakes[i].velocity*elapsed_time;

		// Make the ones that dall off edge wrap around...
		if( flakes[i].position[Y] < -5.0f )
		{
			flakes[i].position[Y] = 5.0f;
			flakes[i].position[X] = ((float)rand( ) / (float)RAND_MAX) * 8.0f;
			if( rand( ) % 2 )
				flakes[i].position[X] = -flakes[i].position[X];
		}
		
	}
}




