/********************************************************************
*	SimSprite.h					Ian Finlayson 2005					*
*																	*
*	Implementation of Sim::Sprite									*
*																	*
********************************************************************/

#include "SimSprite.h"

Sim::Sprite::Animation::Animation( int x, int y, int w, int h, int numFrames, bool left_to_right, SDL_Surface* img, unsigned int msTweenFrames, bool motion, int def_frame )
{
	image = img;
	num_frames = numFrames;
	curr_frame = 0;

	ms_tween_frames = msTweenFrames;
	motionly = motion;
	forward = true;

	default_frame = def_frame;

	timer = SDL_GetTicks( );

	frames = new SDL_Rect[ num_frames ];

	//If the frames are arranged in the image left to right
	if( left_to_right )
	{
		for( int i=0; i<num_frames; i++ )
		{
			frames[ i ].w = w;
			frames[ i ].h = h;
			frames[ i ].y = y;
			
			frames[ i ].x = x + i*w;	//the x moves left to right
		}
	}
	else
	{
		for( int i=(num_frames-1); i>=0; i-- )
		{
			frames[ i ].w = w;
			frames[ i ].h = h;
			frames[ i ].y = y;

			frames[ i ].x = x + i*w;
		}
	}
}

Sim::Sprite::Animation::~Animation( )
{
	delete [] frames;
}

void Sim::Sprite::Animation::Update( )
{
	if( (SDL_GetTicks( ) - timer) > ms_tween_frames )
	{
		if( motionly )
			UpdateMotion( );
		else
			UpdateCircle( );

		timer = SDL_GetTicks( );
	}
}

void Sim::Sprite::Animation::UpdateCircle( )
{
	curr_frame++;
	
	if( curr_frame >= num_frames )
	{
		curr_frame = 0;
	}
}

void Sim::Sprite::Animation::UpdateMotion( )
{
	if( forward )
	{
		if( curr_frame >= (num_frames-1) )
		{
			curr_frame--;
			forward = false;
		}
		else
		{
			curr_frame++;
		}
	}
	else
	{
		if( curr_frame <= 0 )
		{
			curr_frame++;
			forward = true;
		}
		else
		{
			curr_frame--;
		}
	}
}

inline void Sim::Sprite::Animation::Draw( SDL_Surface* screen, SDL_Rect* dest )
{
	SDL_BlitSurface( image, frames + curr_frame, screen, dest );
}

Sim::Sprite::Sprite( int x, int y, int w, int h, int numAnimations )
{
	num_animations = numAnimations;
	curr_animation = 0;

	position.x = x;		position.y = y;
	position.w = w;		position.h =  h;
	X = static_cast<float>( x );
	Y = static_cast<float>( y );

	start_time = end_time = SDL_GetTicks( );

	dX = dY = 0.0f;

	paused = false;

	animations = new Animation*[ num_animations ];
}

Sim::Sprite::~Sprite( )
{
	for( int i=0; i<num_animations; i++ )
	{
		delete animations[i];
	}

	delete animations;
}

void Sim::Sprite::AddAnimation( int x, int y, int numFrames, bool left_to_right, SDL_Surface* img, unsigned int msTweenFlips, bool motion, int default_frame )
{
	animations[ curr_animation ] = new Animation( x, y, position.w, position.h, numFrames, left_to_right, img, msTweenFlips, motion, default_frame );

	curr_animation++;

	if( curr_animation >= num_animations )
	{
		curr_animation = 0;
	}
}

void Sim::Sprite::Update( )
{
	end_time = SDL_GetTicks( );

	unsigned int ms_since_last_frame = end_time - start_time;

	X += dX * (( static_cast<float>(ms_since_last_frame)) / 1000.0f);
	Y += dY * (( static_cast<float>(ms_since_last_frame)) / 1000.0f);

	start_time = end_time;

	position.x = static_cast<int>( X );
	position.y = static_cast<int>( Y );

	if( !paused )
	{
		animations[ curr_animation ]->Update( );
	}
}

void Sim::Sprite::SetAnimation( int anim )
{
	curr_animation = anim;
}

void Sim::Sprite::Draw( SDL_Surface* screen )
{
	animations[ curr_animation ]->Draw( screen, &position );
}

void Sim::Sprite::SetPosition( int x, int y )
{
	position.x = x;
	position.y = y;

	X = static_cast<float>( x );
	Y = static_cast<float>( y );
}

void Sim::Sprite::SetPause( bool pause )
{
	paused = pause;

	if( paused )
	{
		animations[ curr_animation ]->SetToDefault( );
	}
}

