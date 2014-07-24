// Man.cpp
// Ian Finlayson

#include <cstdio>	//for sprintf

#include "Model.h"
#include "BmpTexture.h"
#include "Man.h"

Man::Man( )
{
	char file_name[21];
	for( int i=0; i<NUM_FRAMES; i++ )
	{
		// form the filename...
		sprintf( file_name, "models/Model%i.ms3d", i+1 );

		// Load this frame...
		frames[i] = new Model( file_name );
	}
	current_frame = 0;

	// Load the texture...
	texture = new BmpTexture( "images/texture.bmp" );
}

Man::~Man( )
{
	// Reclaim memory...
	for( int i=0; i<NUM_FRAMES; i++ )
		delete frames[i];

	delete texture;
}

void Man::Draw( )
{
	// Draw the curren frame...
	texture->Bind( );
	frames[current_frame]->Draw( );
}

void Man::Update( float elapsed_time )
{
	// These variables persist...
	static const float MS_TWEEN_FLIPS = 62.5f;
	static float time_start = 0.0f;
	static float time_now = 0.0f;
	static bool forward = true;

	time_now += elapsed_time;

	// If it's time to update...
	if( ((time_now - time_start)*1000.0f) > MS_TWEEN_FLIPS )
	{
		// Update based on whther we're going forward or backward...
		if( forward )
			current_frame++;
		else
			current_frame--;

		// If we went off the edge, start going backwards...
		if( current_frame >= NUM_FRAMES )
		{
			current_frame-=2;
			forward = false;
		}

		// If we go to before 0, start going forwards
		// we skip to frame 10 so the right leg does not go
		// twice in a row...
		else if( current_frame < 0 )
		{
			current_frame = 10;
			forward = true;
		}

		// Update the timer...
		time_start = time_now;
	}
}




