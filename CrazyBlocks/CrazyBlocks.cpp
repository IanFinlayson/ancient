// Main Program Code	-	Copyright Ian Finlayson 2005
// If you are reading this code then I want you to know that
// you will not learn anything about sound software design.
// Some parts of the source code to Crazy Blocks make my eyes
// hurt - and I wrote it. You may, however, learn somthing
// about SDL, or game design in general. Here's one hint - 
// Plan all the features from the begining!!! Not doing this
// was a mistake I made, and it's one reason this code is
// so messy in places. This program is under the GNU GPL meaning
// you are free to alter it and copy it however you like. Enjoy

#include <time.h>
#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "Tet.h"
#include "Board.h"
#include "HighList.h"


// Global variables:
Tet				theTet;
Board			theBoard;

SDL_Surface*	screen;
SDL_Surface*	blocks;
SDL_Surface*	simian;

SDL_Event		event;

int				gameTimer;
int				dropTimer;
unsigned int	ms_tween_drops = 1000;		//start with one second between drops (unsigned to kill a warning)

bool			lost = false;
bool			paused = false;
int				score;
int				level;
int				score_this_level;
SDL_Color		backColor[10];	//changes for each level 1 per level

HighList		highScoreList;


// Function Definitions:
bool Init();
void Quit();
bool HandleGameEvents();
int  RandTet();
void Intro();
void Menu();
int	 HandleMenuEvents();
void Help();
bool HandleHelpEvents();
void ChangeLevel();
std::string GetName();
void PrintText( const char* fontName, const char* text, int x, int y, int size, SDL_Color fore, SDL_Color back );


TTF_Font* arial_15;

// Main:
int main( int argc, char** argv )
{
	Init();

	Intro();

	while( true )	//Do Menu, Game, High Scores forever
	{
		//Reinitialize Everyting
		score = 0;
		score_this_level = 0;
		level = 1;
		lost = false;
		highScoreList.Load();

		theBoard.Init();
		theTet.ReInit( RandTet() );

		Menu();

		while( HandleGameEvents() && !lost )	//HandleGameInput returns false if user quits
		{
			std::stringstream levelS, scoreS;

			while( paused )//While it's paused, we wait for user to unpause or quit.
			{
				if( SDL_PollEvent( &event ) )
				{
					if( event.type == SDL_KEYDOWN )
					{
						if( event.key.keysym.sym == SDLK_p )
						{
							paused = false;	
						}
						else if( event.key.keysym.sym == SDLK_ESCAPE )
						{
							Quit();
							return( 0 );
						}
					}
					else if( event.type == SDL_QUIT )
					{
						Quit();
						return( 0 );
					}
				}
			}//while paused

			if( ( SDL_GetTicks() - gameTimer ) > FRAMERATE )	//if it's time to do somthing
			{
				if( ( SDL_GetTicks() - dropTimer ) > ms_tween_drops )	//if its time to drop'em
				{
					theTet.Move( 0, 16 );	//Drop it
					if( theTet.CollideWalls() || theBoard.Collide( theTet ) )
					{
						theTet.Move( 0, -16 );
						theBoard.Add( theTet );
						theTet.ReInit( RandTet() );	
					}
					dropTimer = SDL_GetTicks();
				}

				int temp = (10*theBoard.DeletRows());
				score += temp;
                score_this_level += temp;

				ChangeLevel();	//Maybe???...
			
				SDL_FillRect( screen, 0, SDL_MapRGB(screen->format, backColor[level-1].r, backColor[level-1].g, backColor[level-1].b ) );	//Clear the Screen to our bgColor

				theBoard.Draw( &blocks, &screen );	//Draw board first
				theTet.Draw( &blocks, &screen );	//Then tetramino

				levelS << "Level: " << level;
				scoreS << "Score: " << score;

				SDL_Color fore;
				if( level != 1 )
				{
					fore.b = fore.g = fore.r = 255;
				}
				else
				{
					fore.b = fore.g = fore.r = 0;
				}

				PrintText( NULL, (const char*)levelS.str().c_str(), 100, 10, 0, fore, backColor[level-1] );
				PrintText( NULL, (const char*)scoreS.str().c_str(), 100, 360, 0, fore, backColor[level-1] );


				SDL_Flip( screen );					//Flip Buffers

				gameTimer = SDL_GetTicks();			//Reset timer
			}
		}

		//Add the User's Score and Show them list...
		std::string theName;

		theName = GetName();

		highScoreList.Add( (const char*)theName.c_str(), score );
		highScoreList.Display( &screen );
		highScoreList.Save();

	}//while( true );

	//Never gets called
	return( 0 );
}

// Function Implementations:
bool Init()
{
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 )
	{
		return( false );
	}

	SDL_WM_SetIcon( SDL_LoadBMP( "CBdata/icon.bmp" ), NULL );	//Set icon thingy

	screen = SDL_SetVideoMode( WINDOWWIDTH, WINDOWHEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
	if( screen == NULL )
	{
		return( false );
	}

	SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL );

	backColor[0].b = backColor[0].g = backColor[0].r = 255;					//Level 1 White
	backColor[1].b = 255;	backColor[1].r = 255;	backColor[1].g = 0;		//Level 2 Pink
	backColor[2].b = 0;		backColor[2].g = 0;		backColor[2].r = 255;	//Level 3 Red
	backColor[3].r = 255;	backColor[3].g = 128;	backColor[3].b = 0;		//Level 4 Orange
	backColor[4].r = 255;	backColor[4].g = 255;	backColor[4].b = 0;		//Level 5 Yellow
	backColor[5].g = 175;	backColor[5].b = 0;		backColor[5].r = 0;		//Level 6 Green
	backColor[6].b = 255;	backColor[6].g = 0;		backColor[6].r = 0;		//Level 7 Blue
	backColor[7].b = 128;	backColor[7].r = 128;	backColor[7].g = 0;		//Level 8 Purple
	backColor[8].b = backColor[8].g = backColor[8].r = 128;					//Level 9 Grey
	backColor[9].b = backColor[9].g = backColor[9].r = 0;					//Level 10 Black
	

	SDL_Surface* temp;
	
	temp = SDL_LoadBMP( "CBdata/blocks.bmp" );		//Loading block image...
	blocks = SDL_DisplayFormat( temp );

	temp = SDL_LoadBMP( "CBdata/simian.bmp" );		//Loading Logo thingy
	simian = SDL_DisplayFormat( temp );


	SDL_FreeSurface( temp );

	if( blocks == NULL )
	{
		return( false );
	}

	SDL_WM_SetCaption( "Crazy Blocks", 0 );

	gameTimer = dropTimer = SDL_GetTicks();

	srand( unsigned(time( NULL )) );				//Seed random values

	TTF_Init();

	arial_15 = TTF_OpenFont( "CBdata/arial.ttf", 15 );

	return( true );
}

void Quit()
{
	TTF_Quit();

	SDL_FreeSurface( blocks );
	SDL_FreeSurface( simian );

	SDL_Quit();
}

bool HandleGameEvents()
{
	if( theBoard.Lost() )	//Check if player lost
	{
		lost = true;
	}

	if( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT )
		{
			Quit();
			exit( 0 );
		}
		if( event.type == SDL_KEYDOWN )
		{
			if( event.key.keysym.sym == SDLK_ESCAPE )
			{
				Quit();
				exit( 0 );
			}
			else if( event.key.keysym.sym == SDLK_UP )
			{
				theTet.Rotate( 90 );
				if( theTet.CollideWalls() || theBoard.Collide( theTet ) )
				{
					theTet.Rotate( 270 );
				}
			}
			else if( event.key.keysym.sym == SDLK_DOWN )
			{
				theTet.Move( 0, 16 );
				if( theTet.CollideWalls() || theBoard.Collide( theTet ) )
				{
					theTet.Move( 0, -16 );
					theBoard.Add( theTet );
					theTet.ReInit( RandTet() );	
				}
			}
			else if( event.key.keysym.sym == SDLK_LEFT )
			{
				theTet.Move( -16, 0 );
				if( theTet.CollideWalls() || theBoard.Collide( theTet ) )
				{
					theTet.Move( 16, 0 );
				}
			}
			else if( event.key.keysym.sym == SDLK_RIGHT )
			{
				theTet.Move( 16, 0 );
				if( theTet.CollideWalls() || theBoard.Collide( theTet ) )
				{
					theTet.Move( -16, 0 );
				}
			}
			else if( event.key.keysym.sym == SDLK_p )
			{
				paused = !paused;
			}
			else
			{
				//We handle no other keys
			}
		}
	}

	return( true );
}

int RandTet()		//Returns a random value between 1 and 7
{
	return( ( ((int)rand()) % 7 ) + 1 );
}


void Intro()
{//This Displays an intro scene for 3 seconds then a menu.
	SDL_Rect desti;
	SDL_FillRect( screen, 0, SDL_MapRGB(screen->format, 255, 255, 255 ) );	//fill to white
	
	desti.h = 100;
	desti.w = 256;

	desti.x = 0;
	desti.y = 100;

	SDL_BlitSurface( simian, NULL, screen, &desti );	//display simian pic

	SDL_Color f,b;
	f.r = f.g = f.b = 0;		//Black fore ground
	b.r = b.g = b.b = 255;		//White background

	PrintText( "CBdata/arial.ttf", "www.angelfire.com/droid/simian", 50, 200, 12, f, b );
	PrintText( "CBdata/adlib.ttf", "PRESENTS...", 45, 225, 25, f, b );

	SDL_Flip( screen );
	SDL_Delay( 2000 );		//Wait 2 seconds
}

void Menu()
{
	//This Function Displays A menu for the user. Most of the code here, however, is for effect
	//It has 3 tets Rotating. Just a menu was not enough...

	SDL_Color red, white;
	red.r = 195;
	red.b = red.g = 0;
	white.b = white.g = white.r = 255;

	Tet showTet1, showTet2, showTet3;
	
	showTet1.ReInit( GREEN );
	showTet1.Move( -80, 256 );	//Left one

	showTet2.ReInit( PURPLE );
	showTet2.Move( 0, 256 );	//Center

	showTet3.ReInit( RED );
	showTet3.Move( 80, 256 );	//Right

	int showTimer = SDL_GetTicks();

	int input = HandleMenuEvents();	

	while( input == 0 )
	{
		input = HandleMenuEvents();

		if( (SDL_GetTicks() - showTimer) > 750 )	//rotate once per (3/4)second
		{
			SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );
			PrintText( "CBdata/adlib.ttf", "CRAZY BLOCKS", 25, 25, 25, red, white );

			PrintText( "CBdata/adlib.ttf", "1. Begin Game", 25, 100, 15, red, white );
			PrintText( "CBdata/adlib.ttf", "2. Help", 25, 120, 15, red, white );
			PrintText( "CBdata/adlib.ttf", "3. Clear Scores", 25, 140, 15, red, white );
			PrintText( "CBdata/adlib.ttf", "4. Quit", 25, 160, 15, red, white );	

			showTet1.Rotate( 90 );	//Rotate them
			showTet2.Rotate( 90 );
			showTet3.Rotate( 90 );

			showTet1.Draw( &blocks, &screen );	//And Draw them
			showTet2.Draw( &blocks, &screen );
			showTet3.Draw( &blocks, &screen );

			SDL_Flip( screen );

			showTimer = SDL_GetTicks();			//Reset timer
		}
	}
}

int HandleMenuEvents()
{
	//Return 0 means stay here
	//Return 1 means play game
	//Return 2 means goto help
	//Return 3 means say "Clearing Scores" for a sec, and do it...
	//Return 4 means Quit

	SDL_Color red, white;
	red.r = 195;
	red.b = red.g = 0;
	white.b = white.g = white.r = 255;

	if( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT )
		{
			Quit();
			exit( 0 );
		}
		else if( event.type == SDL_KEYDOWN )
		{
			if( event.key.keysym.sym == SDLK_1 )
			{
				return( 1 );
			}
			else if( event.key.keysym.sym == SDLK_2 )
			{
				Help();
				return( 1 );
			}
			else if( event.key.keysym.sym == SDLK_3 )
			{
				highScoreList.DeleteList();
				
				PrintText( "CBdata/arial.ttf", "Deleting High Scores...", 15, 200, 12, red, white );

				SDL_Flip( screen );
				SDL_Delay( 1000 );
			}
			else if( event.key.keysym.sym == SDLK_4 )
			{
				Quit();
				exit( 0 );
			}
			else if( event.key.keysym.sym == SDLK_ESCAPE )
			{
				Quit();
				exit( 0 );
			}
		}
	}

	return( 0 );
}

void Help()
{
	SDL_Color red, white;
	red.r = 195;
	red.b = red.g = 0;
	white.b = white.g = white.r = 255;

	Tet showTet1, showTet2, showTet3;

	int showTimer = SDL_GetTicks();

	showTet1.ReInit( YELLOW );
	showTet1.Move( -80, 256 );	//Left one

	showTet2.ReInit( ORANGE );
	showTet2.Move( 16, 256 );	//Center

	showTet3.ReInit( GREY );
	showTet3.Move( 80, 256 );	//Right

	while( HandleHelpEvents() )
	{
		if( ( SDL_GetTicks() - showTimer ) > 750 )
		{
			SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );	//Blank out
			PrintText( "CBdata/adlib.ttf", "CRAZY HELP", 25, 15, 20, red, white );

			PrintText( "CBdata/arial.ttf", "Crazy Blocks is a simple game. Move the"    , 12, 40,  12, red, white );
			PrintText( "CBdata/arial.ttf", "Tetramino with the directional keys. The"   , 12, 60,  12, red, white );
			PrintText( "CBdata/arial.ttf", "more rows you clear the higher your score"  , 12, 80,  12, red, white );
			PrintText( "CBdata/arial.ttf", "The game ends when a piece hits the top."   , 12, 100, 12, red, white );
			PrintText( "CBdata/arial.ttf", "Press 'p' to pause the game"                , 12, 120, 12, red, white );

			PrintText( "CBdata/arial.ttf", "Crazy Blocks is Copyright Ian Finlayson 2005", 12, 150, 12, red, white );
			PrintText( "CBdata/arial.ttf", "Under the GNU General Public License"        , 12, 170, 12, red, white );
			PrintText( "CBdata/arial.ttf", "Art by Caitie Cihak"                         , 12, 190, 12, red, white );

			PrintText( "CBdata/arial.ttf", "Press Enter To Proceed to the Game", 12, 220, 12, red, white );
			PrintText( "CBdata/arial.ttf", "Or Press Escape to Quit"           , 12, 240, 12, red, white );

			showTet1.Rotate( 90 );	//Rotate them
			showTet2.Rotate( 90 );
			showTet3.Rotate( 90 );

			showTet1.Draw( &blocks, &screen );	//And Draw them
			showTet2.Draw( &blocks, &screen );
			showTet3.Draw( &blocks, &screen );

			SDL_Flip( screen );

			showTimer = SDL_GetTicks();
		}
	}
}

bool HandleHelpEvents()
{
	if( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT )
		{
			Quit();
			exit( 0 );
		}
		else if( event.type == SDL_KEYDOWN )
		{
			if( event.key.keysym.sym == SDLK_ESCAPE )
			{
				Quit();
				exit( 0 );
			}
			else if( event.key.keysym.sym == SDLK_RETURN )
			{
				return( false );
			}
		}
	}

	return( true );
}

void ChangeLevel()	//checks for level change
{
	if( level == 10 )//Only 10 levels
	{
		return;
	}

	if( score_this_level >= 100)
	{
		level++;
		score_this_level -= 100;

		ms_tween_drops -= 100;	//Drops come 100 ms faster!!!
	}

}

std::string GetName()
{
	std::string name;

	bool done = false;

	SDL_Color red,white;
	red.r = 195;
	red.b = red.g = 0;
	white.b = white.g = white.r = 255;

	while( !done )
	{
		SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );

		PrintText( "CBdata/adlib.ttf", "You Have A High Score",   10,  10, 18, red, white );
		PrintText( "CBdata/arial.ttf", "Name: ",                  10,  50, 12, red, white );

		if( name.c_str() != NULL )
		{
			PrintText( "CBdata/arial.ttf", (const char*)name.c_str(), 50, 50, 12, red, white );
		}

		PrintText( "CBdata/arial.ttf", "Congratulations", 10, 250, 15, red, white );
		PrintText( "CBdata/arial.ttf", "Press enter when you are done", 10, 270, 15, red, white );

		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
			{
				Quit();
				exit( 0 );
			}
			if( event.type == SDL_KEYDOWN )
			{
				if( event.key.keysym.sym == SDLK_RETURN )
				{
					done = true;
				}
				else if( (( event.key.keysym.sym < 97 ) || ( event.key.keysym.sym > 122 )) && (event.key.keysym.sym != SDLK_BACKSPACE) )
				{
				
				}
				else
				{
					if( event.key.keysym.sym == SDLK_BACKSPACE )
					{
						char* temp = (char*)name.c_str();
	
						temp[name.size()-1] = '\0';

						name = temp;
					}
					else if( name.empty() )	//We will capitalize first letter
					{
						name.append( 1, event.key.keysym.sym - 32 );
					}
					else
					{
						name.append( 1, event.key.keysym.sym );
					}
				}
			}
		}

		SDL_Flip( screen );
	}

	return( name );
}

void PrintText( const char* fontName, const char* text, int x, int y, int size, SDL_Color fore, SDL_Color back )
{
	TTF_Font* theFont = NULL;

	if( fontName )
	{
		theFont = TTF_OpenFont( fontName, size );
	}
	else
	{
		theFont = arial_15;
	}

	SDL_Surface* temp;
	temp = TTF_RenderText_Shaded( theFont, text, fore, back );

	SDL_Rect desti;
	desti.h = 0;
	desti.w = 0;
	desti.x = x;
	desti.y = y;
	
	SDL_BlitSurface( temp, NULL, screen, &desti );

	SDL_FreeSurface( temp );

	if( fontName )
	{
		TTF_CloseFont( theFont );
	}
}


