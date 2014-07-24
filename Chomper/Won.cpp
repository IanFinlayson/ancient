// This function is called when the
// user beats the game

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include "SimTextWriter.h"
#include "SimTextField.h"
#include "SimButton.h"

#include "Definitions.h"
#include "Functions.h"

struct HighScore
{
	char name[25];
	int score;

	bool you;

	//Defines relational operators to make sort work...
	//Ummm sort does ascending order, but I wanted descending
	//rather than find a proper solution, I put ! in front
	//of everything...I was tired...
	bool operator<( const HighScore & rhs ) const
	{
		return !(this->score < rhs.score);
	}
	bool operator>( const HighScore & rhs ) const
	{
		return !(this->score > rhs.score);
	}
	bool operator<=( const HighScore & rhs ) const
	{
		return !(this->score <= rhs.score);
	}
	bool operator>=( const HighScore & rhs ) const
	{
		return !(this->score >= rhs.score);
	}
	bool operator==( const HighScore & rhs ) const
	{
		return !(this->score == rhs.score);
	}
	bool operator!=( const HighScore & rhs ) const
	{
		return !(this->score != rhs.score);
	}
};

Chomp::GameState Chomp::Won( SDL_Surface* screen, int score, bool won )
{
	//We load the top 10 scores into a vector
	std::vector< HighScore > high_scores;
	
	std::fstream fileIn( "CHdata/scores.dat", std::ios::in );
	HighScore temp;
	do
	{
		fileIn >> temp.name;
		fileIn >> temp.score;

		temp.you = false;

		high_scores.push_back( temp );

	}while( !fileIn.eof( ) );

	fileIn.close( );

	//Add "you"
	HighScore you;
	strcpy( you.name, "Your_Name" );
	you.score = score;
	you.you = true;

	high_scores.push_back( you );

	//Sort the thing by score...(ascending order)
	std::sort( high_scores.begin( ), high_scores.end( ) );
	char score_string[60];
	char temp_num[10];

	Sim::TextWriter tw( 2 );
	int FREE_SANS_14 = tw.AddFont( "CHdata/FreeSans.ttf", 14 );
	int AIRMOLE_24 = tw.AddFont( "CHdata/airmole.ttf", 24 );

	char message[60];
	sprintf( message, "You achieved a score of %i. Please enter your name!", score );

	Sim::TextField tf( 30, 100, 340, 25, 1, 0,0,128, tw.GetFont( FREE_SANS_14 ), 25, 150,175,255, 0,0,0, 5, 0, "Your_Name", true );

	Sim::Button modify( 30, 135, 100, 27, 1, 0,0,128, 10, 2 );
	modify.SetBackGround( 150,175,255 );
	modify.SetForeGround( "Set Name", tw.GetFont( FREE_SANS_14 ), 0,0,0 );

	Sim::Button menu( 150, 135, 100, 27, 1, 0,0,128, 10, 2 );
	menu.SetBackGround( 150,175,255 );
	menu.SetForeGround( "Main Menu", tw.GetFont( FREE_SANS_14 ), 0,0,0 );

	Sim::Button quit( 270, 135, 100, 27, 1, 0,0,128, 30, 2 );
	quit.SetBackGround( 150,175,255 );
	quit.SetForeGround( "Quit", tw.GetFont( FREE_SANS_14 ), 0,0,0 );

	SDL_Event event;
	while( true )
	{
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
			{
				return Chomp::QUIT;
			}
			else if( event.type == SDL_KEYDOWN )
			{
				if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					return Chomp::MENU;
				}
				else if( event.key.keysym.sym == SDLK_SPACE )
				{
					SDL_keysym und;
					und.sym = SDLK_UNDERSCORE;

					tf.HandleKeyDown( und );
				}
				else
				{
					tf.HandleKeyDown( event.key.keysym );
				}
			}
			else if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				tf.HandleMouseDown( event.button.x, event.button.y );
				menu.HandleMouseDown( event.button.x, event.button.y );
				modify.HandleMouseDown( event.button.x, event.button.y );
				quit.HandleMouseDown( event.button.x, event.button.y );
			}
			else if( event.type == SDL_MOUSEBUTTONUP )
			{
				tf.HandleMouseUp( event.button.x, event.button.y );

				if( modify.HandleMouseUp( event.button.x, event.button.y ) )
				{
					for( unsigned int i=0; i<high_scores.size(); i++ )
					{
						if( high_scores[i].you )	//if the score is you
						{
							strcpy( high_scores[i].name, tf.GetText().c_str() );
						}
					}

					std::fstream fileOut( "CHdata/scores.dat", std::ios::out );

					for( unsigned int i=0; i<high_scores.size( ); i++ )
					{
						if( i < 10 )	//only right the top 10
						{
							fileOut << std::endl << high_scores[i].name << " " << high_scores[i].score;							
						}
					}

					fileOut.close( );
				}

				if( menu.HandleMouseUp( event.button.x, event.button.y ) )
				{
					return Chomp::MENU;
				}

				if( quit.HandleMouseUp( event.button.x, event.button.y ) )
				{
					return Chomp::QUIT;
				}
			}
		}

		SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0,0,0 ) );

		if( won )
		{
			tw.WriteText( screen, AIRMOLE_24, "Congratulations, you won!", 30, 34, 150, 175, 255, 0, 0, 0 );
		}
		else
		{
			tw.WriteText( screen, AIRMOLE_24, "Sorry, you lost!", 30, 34, 150, 175, 255, 0, 0, 0 );
		}
		tw.WriteText( screen, FREE_SANS_14, message, 30, 75, 150, 175, 255, 0, 0, 0 );

		tf.Draw( screen );
		modify.Draw( screen );
		menu.Draw( screen );
		quit.Draw( screen );



		tw.WriteText( screen, AIRMOLE_24, "HighScores", 420, 30, 150,175,255, 0,0,0 );
		for( unsigned int i=0; i<high_scores.size(); i++ )
		{
			//We only want top 10 displayed
			if( i > 9 )
			{
				break;
			}

			sprintf( temp_num, "%i", i+1 );
			strcpy( score_string, temp_num );
			strcat( score_string, ": " );
			strcat( score_string, high_scores[i].name );

			if( !high_scores[i].you )
				tw.WriteText( screen, FREE_SANS_14, score_string, 420, 70 + 20*i, 150,175,255, 0,0,0 );
			else
				tw.WriteText( screen, FREE_SANS_14, score_string, 420, 70 + 20*i, 255,255,255, 0,0,0 );

			sprintf( temp_num, "%i", high_scores[i].score );

			if( !high_scores[i].you )
				tw.WriteText( screen, FREE_SANS_14, temp_num, 660, 70 + 20*i, 150, 175, 255, 0,0,0 );
			else
				tw.WriteText( screen, FREE_SANS_14, temp_num, 660, 70 + 20*i, 255, 255, 255, 0,0,0 );
		}

		SDL_Flip( screen );
	}
}
