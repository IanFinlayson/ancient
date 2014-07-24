// Ghost.cpp Implements ghost functionality
// Copyright Ian Finlayson 2005

#include <cstdlib>

#include <SDL/SDL.h>

#include "SimSprite.h"
#include "SimImage.h"

#include "Definitions.h"
#include "Level.h"
#include "Player.h"
#include "Ghost.h"

Chomp::Ghost::Ghost( int x, int y, SDL_Surface* ghost_img, SDL_Surface* pupil_img, int ghost_color, Level* the_level, Player* the_player )
{
	sprite = new Sim::Sprite( x, y, 32, 32, 2 );
	
	this->ghost_color = ghost_color;

	current_state = BASIC;

	pupil.img = pupil_img;

	sprite->AddAnimation( 64*(ghost_color), 0, 2, true, ghost_img, 250, false );
	sprite->AddAnimation( 192, 0, 2, true, ghost_img, 250, false );

	//Animation 0 is normal - Animation 1 is blue/avoid state

	level = the_level;
	player = the_player;

	dead = false;

	speed = static_cast<float>( NORMAL_SPEED );
	sprite->SetAnimation( 0 );

	paused = false;
	is_blue = false;

	time_blue_ends = 0;
}

Chomp::Ghost::~Ghost( )
{
	delete sprite;
}

int Chomp::Ghost::Update( )
{
	if( paused )
	{
		return 0;
	}

	if( is_blue )
	{
		if( SDL_GetTicks( ) > time_blue_ends )
		{
			is_blue = false;
			current_state = BASIC;
			speed = static_cast<float>( NORMAL_SPEED );

			sprite->SetAnimation( 0 );

			if( dead )
			{
				dead = false;
				
				int x,y;
				level->GetGhostysStart( &x, &y );
				sprite->SetPosition( x, y );
			}
		}
	}

	int x = sprite->GetPosition( ).x;
	int y = sprite->GetPosition( ).y;

	int x_rel = (x - 4) % TILE_SIZE;
	int y_rel = (y - 4) % TILE_SIZE;

	if( ( x_rel <= 4 ) && ( y_rel <= 4 ) )	//If aligned
	{
		if( !dead )
		{
			//Set movement
			if( next_direction == EAST )
			{
				sprite->SetMovement( speed, 0.0f );
			}
			else if( next_direction == NORTH )
			{
				sprite->SetMovement( 0.0f, -speed );
			}
			else if( next_direction == WEST )
			{
				sprite->SetMovement( -speed, 0.0f );
			}
			else	//SOUTH
			{
				sprite->SetMovement( 0.0f, speed );
			}

			//Find next direction
			if( ghost_color == 0 )	//red
			{
				next_direction = GetNextRed( );
			}
			else if( ghost_color == 1 )	//yellow
			{
				next_direction = GetNextYellow( );
			}
			else if( ghost_color == 2 )	//purple
			{
				next_direction = GetNextPurple( );
			}
		}
	}

	//Set pupil position...
	if( next_direction == EAST )
	{
		pupil.location.x = x + 13;
		pupil.location.y = y + 10;
	}
	else if( next_direction == NORTH )
	{
		pupil.location.x = x + 10;
		pupil.location.y = y + 7;
	}
	else if( next_direction == WEST )
	{
		pupil.location.x = x + 7;
		pupil.location.y = y + 10;
	}
	else	//SOUTH
	{
		pupil.location.x = x + 10;
		pupil.location.y = y + 13;
	}


    if( current_state != AVOID )	//not "blue"
	{
		sprite->SetAnimation( 0 );
	}
	else	//blue
	{
		sprite->SetAnimation( 1 );
	}

	sprite->Update( );

	if( !dead )
	{
    SDL_Rect g = sprite->GetPosition( ), m = player->GetPosition( );
		bool collision = Collide(&g, &m);

		if( !is_blue && collision )
		{
			return 1;
		}
		else if( collision )
		{
			// The ghost has died

			dead = true;

			//We make the ghosts go straight back to his starting point!

			double dx,dy;//Vector to go to

			int Ex,Ey;	//Ending coords
			level->GetGhostysStart( &Ex, &Ey );

			int Sx,Sy;	//Starting coords;
			Sx = sprite->GetPosition().x;
			Sy = sprite->GetPosition().y;

			double denominator = time_blue_ends - SDL_GetTicks( );

			if( denominator != 0 )
			{
				dx = ((float)(Ex - Sx) / (denominator/1000.0f));
				dy = ((float)(Ey - Sy) / (denominator/1000.0f));
			}

			sprite->SetMovement( (float)dx, (float)dy );

			return 2;
		}
	}

	return 0;
}

void Chomp::Ghost::Draw( SDL_Surface* screen )
{
	sprite->Draw( screen );
	SDL_BlitSurface( pupil.img, NULL, screen, &pupil.location );
}

Chomp::Direction Chomp::Ghost::GetNextPurple( )
{
	//First see if we have line of sight...
	int gx = sprite->GetPosition().x / TILE_SIZE;
	int gy = sprite->GetPosition().y / TILE_SIZE;
	int px = player->GetPosition().x / TILE_SIZE;
	int py = player->GetPosition().y / TILE_SIZE;

	int relx = px - gx;
	int rely = py - gy;

	if( current_state != AVOID )
	{
		if( relx == 0 ) //Allinged vertically
		{
			if( rely > 0 )
			{
				if( level->CanGo( gx, gy, SOUTH ) )
				{
					return SOUTH;
				}
			}
			else
			{
				if( level->CanGo( gx, gy, NORTH ) )
				{
					return NORTH;
				}
			}
		}
		if( rely == 0 ) //Alligned horizontally
		{
			if( relx > 0 )
			{
				if( level->CanGo( gx, gy, EAST ) )
				{
					return EAST;
				}
			}
			else
			{
				if( level->CanGo( gx, gy, WEST ) )
				{
					return WEST;
				}
			}
		}
	}

	// If we got here, there is no line of sight or we're in avoid...

	if( current_state == AVOID )
	{
		// Go opposite direction

		Direction player_head = player->GetHeading( );

		if( level->CanGo( gx, gy, (player_head+2) % 4 ) )
		{
			return (player_head + 2) % 4;
		}
	}
	else
	{
		Direction player_head = player->GetHeading( );
		
		if( level->CanGo( gx, gy, player_head ) )
		{
			return player_head;
		}
	}

	//If we got here, just go randomly...

	srand( (SDL_GetTicks())/500 );

	Direction dir;
	do
	{
		dir = rand( ) % 4;

	}while( !level->CanGo( gx, gy, dir ) );

	return dir;
}

Chomp::Direction Chomp::Ghost::GetNextRed( )
{
	float gx = (float)sprite->GetPosition( ).x;
	float gy = (float)sprite->GetPosition( ).y;
	float px = (float)player->GetPosition( ).x;
	float py = (float)player->GetPosition( ).y;

	float relx = px - gx;
	float rely = py - gy;

	Direction go[4];

	if( abs(relx) > abs(rely) )	//Major axis is horizontal
	{
		if( relx > 0 )
		{
			go[0] = EAST;
			go[3] = WEST;
		}
		else
		{
			go[0] = WEST;
			go[3] = EAST;
		}

		if( rely > 0 )
		{
			go[1] = SOUTH;
			go[2] = NORTH;
		}
		else
		{
			go[1] = NORTH;
			go[2] = SOUTH;
		}
	}
	else	// Major axis is vertical
	{
		if( rely > 0 )
		{
			go[0] = SOUTH;
			go[3] = NORTH;
		}
		else
		{
			go[0] = NORTH;
			go[3] = SOUTH;
		}

		if( relx > 0 )
		{
			go[1] = EAST;
			go[2] = WEST;
		}
		else
		{
			go[1] = WEST;
			go[2] = EAST;
		}
	}

	int x = sprite->GetPosition( ).x / TILE_SIZE;
	int y = sprite->GetPosition( ).y / TILE_SIZE;

	if( current_state == AVOID )
	{
		//We must reverse the order
		Direction temp = go[0];
		go[0] = go[3];
		go[3] = temp;

		temp = go[1];
		go[1] = go[2];
		go[2] = temp;
	}

	for( int i=0; i<4; i++ )
	{
		if( level->CanGo( x, y, go[i] ) )
		{
			return go[i];
		}
	}


	return NONE;
}

Chomp::Direction Chomp::Ghost::GetNextYellow( )
{
	int gx = sprite->GetPosition().x / TILE_SIZE;
	int gy = sprite->GetPosition().y / TILE_SIZE;
	int px = player->GetPosition().x / TILE_SIZE;
	int py = player->GetPosition().y / TILE_SIZE;

	int relx = px - gx;
	int rely = py - gy;

	if( current_state != AVOID )
	{
		if( relx == 0 ) //Allinged vertically
		{
			if( rely > 0 )
			{
				if( level->CanGo( gx, gy, SOUTH ) )
				{
					return SOUTH;
				}
			}
			else
			{
				if( level->CanGo( gx, gy, NORTH ) )
				{
					return NORTH;
				}
			}
		}
		if( rely == 0 ) //Alligned horizontally
		{
			if( relx > 0 )
			{
				if( level->CanGo( gx, gy, EAST ) )
				{
					return EAST;
				}
			}
			else
			{
				if( level->CanGo( gx, gy, WEST ) )
				{
					return WEST;
				}
			}
		}
	}

	//Return random direction
	srand( (SDL_GetTicks()+3000)/500 );

	Direction dir;
	do
	{
		dir = rand( ) % 4;

	}while( !level->CanGo( gx, gy, dir ) );

	return dir;
}

void Chomp::Ghost::SetPosition( int x, int y )
{
	sprite->SetPosition( x, y );
}

void Chomp::Ghost::TurnBlue( )
{
	if( !dead )
	{
		is_blue = true;
		current_state = AVOID;

		time_blue_ends = SDL_GetTicks( ) + TIME_BLUE;

		speed = static_cast<float>( BLUE_SPEED );

		sprite->SetAnimation( 1 );
	}
	else
	{
		time_blue_ends = SDL_GetTicks( ) + TIME_BLUE;

		//We must reset their velocity so that they end up in the right place
		double dx,dy;//Vector to go to

		int Ex,Ey;	//Ending coords
		level->GetGhostysStart( &Ex, &Ey );

		int Sx,Sy;	//Starting coords;
		Sx = sprite->GetPosition().x;
		Sy = sprite->GetPosition().y;

		double denominator = time_blue_ends - SDL_GetTicks( );

		if( denominator != 0 )
		{
			dx = ((float)(Ex - Sx) / (denominator/1000.0f));
			dy = ((float)(Ey - Sy) / (denominator/1000.0f));
	}

		sprite->SetMovement( (float)dx, (float)dy );

	}
}

void Chomp::Ghost::Pause( )
{
	paused = true;

	last = next_direction;
	next_direction = NONE;

	dx_saved = sprite->GetDx( );
	dy_saved = sprite->GetDy( );

	sprite->SetMovement( 0.0f, 0.0f );
	sprite->SetPause( true );

	time_pause_started = SDL_GetTicks( );
}

void Chomp::Ghost::Unpause( )
{
	//We must call this once here to clear out the old time values before setting the
	//movement back. Not doing so will cause the ghosts to do a "warp" jump across the screen!
	sprite->Update( );

	paused = false;

	next_direction = last;

	sprite->SetPause( false );

	sprite->SetMovement( dx_saved, dy_saved );

	//We must make sure pause does not screw up the power bonus!

	if( is_blue )
	{
		unsigned int time_paused = SDL_GetTicks( ) - time_pause_started;

		time_blue_ends += time_paused;
	}
}

void Chomp::Ghost::Validate( )
{
	int x = sprite->GetPosition( ).x;
	int y = sprite->GetPosition( ).y;

	if( (x < 0) || (x > 600) )
	{
		int x,y;

		level->GetGhostysStart( &x, &y );
		SetPosition( x, y );
	}

	if( (y < 0) || (y > 600) )
	{
		int x,y;

		level->GetGhostysStart( &x, &y );
		SetPosition( x, y );
	}
}

void Chomp::Ghost::Restart( )
{
	dead = false;

	speed = static_cast<float>( NORMAL_SPEED );
	sprite->SetAnimation( 0 );

	current_state = BASIC;

	paused = false;
	is_blue = false;

	time_blue_ends = 0;
}

