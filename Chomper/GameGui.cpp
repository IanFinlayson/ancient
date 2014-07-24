// This file separates the user interface stuff used
// by the game() function from the actual game code
// Copyright Ian Finlayson 2005

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "SimButton.h"
#include "SimTextWriter.h"

#include "GameGui.h"


Chomp::GameGui::GameGui( )
{
	paused = false;

	tw = new Sim::TextWriter( 2 );

	AIRMOLE_24 = tw->AddFont( "CHdata/airmole.ttf", 24 );
	FREE_SANS_14 = tw->AddFont( "CHdata/FreeSans.ttf", 14 );

	pause = new Sim::Button( 620, 550, 70, 25, 1, 0,0,128, 8,0 );
	menu = new Sim::Button( 710, 550, 70, 25, 1, 0, 0, 128, 16,0 );

	pause->SetBackGround( 150, 175, 255 );
	menu->SetBackGround( 150, 175, 255 );

	pause->SetForeGround( "Pause", tw->GetFont( FREE_SANS_14 ), 0,0,0 );
	menu->SetForeGround( "Menu", tw->GetFont( FREE_SANS_14 ), 0,0,0 );

	speed_bonus = power_bonus = false;
}

void Chomp::GameGui::PrepareForLevelScreen( SDL_Surface* screen, int level, int lev_score )
{
	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0,0,0 ) );

	char lev_string[45];
	char point_str[45];

	if( level != 1 )	//We don't want to congratulate someone when they have not even beaten level 1!
	{
		sprintf( lev_string, "Congratulations!   Get Ready For Level %i", level );
		sprintf( point_str, "Time Bonus for level %i:   %i", level-1, lev_score );

		tw->WriteText( screen, AIRMOLE_24, lev_string, 195, 240, 150,175,255, 0,0,0 );
		tw->WriteText( screen, FREE_SANS_14, point_str, 290, 280, 150,175,255, 0,0,0 );
	}
	else
	{
		sprintf( lev_string, "Get Ready For Level %i", level );

		tw->WriteText( screen, AIRMOLE_24, lev_string, 280, 240, 150,175,255, 0,0,0 );	
	}
	SDL_Flip( screen );
	
	SDL_Delay( 2500 );
}

void Chomp::GameGui::Draw( SDL_Surface* screen, int score, int lev_num, int lives )
{
	if( !paused )
	{
		num_power_show = 0;
		num_speed_show = 0;


		if( power_bonus )
		{
			if( SDL_GetTicks() >= time_power_ends )
			{
				power_bonus = false;
			}
			else
			{
				float percent_left = ((time_power_ends - SDL_GetTicks()) / (float)TIME_ON_POWER_BONUS) * 100.0f;

				num_power_show = (((int)percent_left) / MAX_THINGYS_TO_SHOW) + 1;
			}
		}

		if( speed_bonus )
		{
			if( SDL_GetTicks() >= time_speed_ends )
			{
				speed_bonus = false;
			}
			else
			{
				float percent_left = ((time_speed_ends - SDL_GetTicks()) / (float)TIME_ON_SPEED_BONUS) * 100.0f;

				num_speed_show = (((int)percent_left) / MAX_THINGYS_TO_SHOW) + 1;
			}
		}
	}

	char temp[16];

	tw->WriteText( screen, AIRMOLE_24, "Chomper", 645, 10, 150,175,255, 0,0,0 );

	sprintf( temp, "Score: %i", score );
	tw->WriteText( screen, FREE_SANS_14, temp, 620, 60, 150,175,255, 0,0,0 );

	sprintf( temp, "Level: %i", lev_num );
	tw->WriteText( screen, FREE_SANS_14, temp, 620, 82, 150,175,255, 0,0,0 );

	sprintf( temp, "Lives: %i", lives - 1 );	//it's lives - 1 because it displays lives *left*
	tw->WriteText( screen, FREE_SANS_14, temp, 620, 104, 150,175,255, 0,0,0 );


	tw->WriteText( screen, FREE_SANS_14, "Power Bonus:", 620, 200, 150,175,255, 0,0,0 );
	for( int i=0; i<num_power_show; i++ )
	{
		tw->WriteText( screen, AIRMOLE_24, "*", 610 + (i*18), 225, 200,0,0, 0,0,0 );
	}

	tw->WriteText( screen, FREE_SANS_14, "Speed Bonus:", 620, 300, 150,175,255, 0,0,0 );
	for( int i=0; i<num_speed_show; i++ )
	{
		tw->WriteText( screen, AIRMOLE_24, "*", 610 + (i*18), 325, 200,200,0, 0,0,0 );
	}

	pause->Draw( screen );
	menu->Draw( screen );
}

void Chomp::GameGui::HandleMouseDown( int mx, int my )
{
	pause->HandleMouseDown( mx, my );
	menu->HandleMouseDown( mx, my );

}

int Chomp::GameGui::HandleMouseUp( int mx, int my )
{
	if( pause->HandleMouseUp( mx, my ) )
	{
		if( paused )	//User clicked UnPause
		{
			paused = false;
			pause->ChangeButtonText( "Pause" );

			return 2;
		}
		else	//User clicked Pause
		{
			paused = true;
			pause->ChangeButtonText( "Unpause" );

			return 1;
		}
	}

	if( menu->HandleMouseUp( mx, my ) )
	{
		return 3;
	}

	return 0;
}

void Chomp::GameGui::KillScreen( SDL_Surface* screen )
{
	speed_bonus = false;
	power_bonus = false;

	tw->WriteText( screen, AIRMOLE_24, "Try Again", 250, 285, 150, 175, 255 );

	SDL_Flip( screen );

	SDL_Delay( 1750 );
}

void Chomp::GameGui::PowerBonusOn( )
{
	power_bonus = true;
	
	time_power_ends = SDL_GetTicks() + TIME_ON_POWER_BONUS;
}

void Chomp::GameGui::SpeedBonusOn( )
{
	speed_bonus = true;

	time_speed_ends = SDL_GetTicks() + TIME_ON_SPEED_BONUS;
}

void Chomp::GameGui::Pause( )
{
	pause_started = SDL_GetTicks( );
}

void Chomp::GameGui::Unpause( )
{
	unsigned int length_of_pause = SDL_GetTicks( ) - pause_started;

	time_speed_ends += length_of_pause;
	time_power_ends += length_of_pause;
}
