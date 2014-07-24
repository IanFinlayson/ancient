// Implements Player class
// Copyright Ian Finlayson 2005

#include <SDL/SDL.h>

#include "SimSprite.h"
#include "SimImage.h"
#include "Definitions.h"
#include "Level.h"
#include "Player.h"

Chomp::Player::Player( Level* lev )
{
	level = lev;	//Set our pointer to the level

	int x,y;

	level->GetChomperStart( &x, &y );	//find where chomper starts

	sprite = new Sim::Sprite( x, y, 32, 32, NUM_ANIMATIONS );

	foreward = new Sim::Image( "CHdata/chomper.png", 0,0,255 );

	backward = new Sim::Image( );

	//The backwards image is the foreward flipped both ways.
	*backward = foreward->GetMirroredX( ).GetMirroredY( );

	sprite->AddAnimation( 0, 0, NUM_FRAMES_PER_ANIM, true, foreward->image, FRAME_RATE, true );		//East
	sprite->AddAnimation( 128, 0, NUM_FRAMES_PER_ANIM, true, foreward->image, FRAME_RATE, true );	//North
	sprite->AddAnimation( 128, 0, NUM_FRAMES_PER_ANIM, false, backward->image, FRAME_RATE, true );	//West
	sprite->AddAnimation( 0, 0, NUM_FRAMES_PER_ANIM, false, backward->image, FRAME_RATE, true );	//South

	next_direction =  NONE;
	sprite->SetAnimation( EAST );

	NORMAL_SPEED = 150.0f;
	BONUS_SPEED = 250.0f;

	on_bonus_speed = false;

	points = 0;
	lives = STARTING_LIVES;
}

Chomp::Player::~Player( )
{
	delete sprite;
	delete foreward;
	delete backward;
}

void Chomp::Player::Draw( SDL_Surface* screen )
{
	sprite->Draw( screen );
}

int Chomp::Player::Update( )
{
	if( lives == 0 )	//Lost game
	{
		return 3;
	}

	int return_val = 0;

	int x = sprite->GetPosition().x;
	int y = sprite->GetPosition().y;

	int x_rel = (x - 4) % TILE_SIZE;
	int y_rel = (y - 4) % TILE_SIZE;

	float speed;

	if( !paused )
	{
		if( on_bonus_speed )
		{
			if( SDL_GetTicks() >= time_bonus_ends )
			{
				on_bonus_speed = false;
			}

			speed = BONUS_SPEED;
		}
		else
		{
			speed = NORMAL_SPEED;
		}
	}

	if( ( x_rel <= 3 ) && ( y_rel <= 3 ) )
	{
		if( next_direction == NONE )
		{
			sprite->SetMovement( 0.0f, 0.0f );
		}
		else if( next_direction == EAST )
		{
			if( level->CanGo( x / TILE_SIZE, y / TILE_SIZE, EAST ) )
			{
				sprite->SetAnimation( EAST );
				sprite->SetMovement( speed, 0.0f );
			}
			else
			{
				if( !level->CanGo( x / TILE_SIZE, y / TILE_SIZE, sprite->GetAnimation( ) ) )
				{
					sprite->SetMovement( 0.0f, 0.0f );
				}
			}
		}
		else if( next_direction == NORTH )
		{
			if( level->CanGo( x / TILE_SIZE, y / TILE_SIZE, NORTH ) )
			{
				sprite->SetAnimation( NORTH );
				sprite->SetMovement( 0.0f, -speed );
			}
			else
			{
				if( !level->CanGo( x / TILE_SIZE, y / TILE_SIZE, sprite->GetAnimation( ) ) )
				{
					sprite->SetMovement( 0.0f, 0.0f );
				}			
			}
		}
		else if( next_direction == WEST )
		{
			if( level->CanGo( x / TILE_SIZE, y / TILE_SIZE, WEST ) )
			{
				sprite->SetAnimation( WEST );
				sprite->SetMovement( -speed, 0.0f );
			}
			else
			{
				if( !level->CanGo( x / TILE_SIZE, y / TILE_SIZE, sprite->GetAnimation( ) ) )
				{
					sprite->SetMovement( 0.0f, 0.0f );
				}			
			}
		}
		else
		{
			if( level->CanGo( x / TILE_SIZE, y / TILE_SIZE, SOUTH ) )
			{
				sprite->SetAnimation( SOUTH );
				sprite->SetMovement( 0.0f, speed );
			}
			else
			{
				if( !level->CanGo( x / TILE_SIZE, y / TILE_SIZE, sprite->GetAnimation( ) ) )
				{
					sprite->SetMovement( 0.0f, 0.0f );
				}			
			}
		}
	}

	int pellet = level->HandlePellet( x, y );

	if( pellet ==  1 )	//Normal
	{
		points += POINTS_PER_PELLET;
	}
	else if( pellet == 2 ) //Power pellet
	{
		return_val = 1;
	}
	else if( pellet == 3 ) //speed pellet
	{
		on_bonus_speed = true;
		time_bonus_ends = SDL_GetTicks( ) + TIME_ON_BONUS_SPEED;

		return_val = 2;
	}
	else if( pellet == 4 ) //extra life pellet - with point bonus
	{
		lives++;
		points += POINTS_PER_BONUS;
	}

	sprite->Update( );

	return return_val;
}

void Chomp::Player::SetNextDirection( Direction dir )
{
	next_direction = dir;
}

void Chomp::Player::ResetChomper( )
{
	int x,y;
	level->GetChomperStart( &x, &y );

	sprite->SetPosition( x, y );
	on_bonus_speed = false;

	sprite->SetPause( false );
	sprite->SetAnimation( EAST );

	next_direction = NONE;;
}

void Chomp::Player::PauseChomper( )
{
	last_direction = next_direction;
	sprite->SetPause( true );
	next_direction = Chomp::NONE;

	time_of_pause = SDL_GetTicks( );

	paused = true;
}

void Chomp::Player::UnpauseChomper( )
{
	sprite->SetPause( false );
	next_direction = last_direction;

	unsigned int paused_time = SDL_GetTicks( ) - time_of_pause;

	time_bonus_ends += paused_time;

	paused = false;
}

int Chomp::Player::GetScore( )
{
	return points;
}

int Chomp::Player::GetLives( )
{
	return lives;
}

void Chomp::Player::Kill( )
{
	lives--;
}

void Chomp::Player::Validate( )
{
	int x = sprite->GetPosition( ).x;
	int y = sprite->GetPosition( ).y;

	if( (x < 0) || (x > 600) )
	{
		int x,y;

		level->GetChomperStart( &x, &y );
		sprite->SetPosition( x, y );
	}

	if( (y < 0) || (y > 600) )
	{
		int x,y;

		level->GetChomperStart( &x, &y );
		sprite->SetPosition( x, y );
	}
}


void Chomp::Player::ClearScore( )
{
	lives = STARTING_LIVES;

	points = 0;
}

void Chomp::Player::AddToScore( int sco )
{
	points += sco;
}

Chomp::Direction Chomp::Player::GetHeading( )
{
	return sprite->GetAnimation( );
}
