// Bombs Away! - A BreakOut clone	-	Copyright Ian Finlayson 2005

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
//#include <iostream>
#include <sstream>

#include "Constants.h"
#include "Level.h"
#include "Entity.h"

bool Init( );
void ReInit( );
void Quit( );


// GLOBAL DATA
Entity*			paddle;
Entity*			ball;
Level*			level;

SDL_Surface*	screen;
SDL_Surface*	blocksS;
SDL_Surface*	paddleS;
SDL_Surface*	ballS;
SDL_Surface*	bombS;

SDL_Event		event;

bool			moveBallWithPaddle	=	true;
int				timer;

bool			won					=	false;
bool			lost				=	false;
bool			wonGame				=	false;
bool			lostGame			=	false;

int				levelNum			=	1;

int				score				=	0;

int				lives				=	3;
int				blasts				=	3;
int				pauses				=	3;
int				reverses			=	3;

bool			paused				=	false;
int				pausedTimer			=	0;
int				oldX, oldY;
bool			blasted				=	false;
bool			reversed			=	false;

bool			gameIsPaused		=	false;	//Not to be confused with the "pause power"

float speed_x, speed_y;
TTF_Font*		arial12				=	NULL;

// Function pointers will control flow
	void(*DoThis)( );			void(*HandleEvents)( );
//	-----------------			-----------------------  //
	void Intro( );				void HandleIntro( );
	void Menu( );				void HandleMenu( );
	void Help( );				void HandleHelp( );
	void Game( );				void HandleGame( );
	void Won( );				void HandleWon( );
	void Lost( );				void HandleLost( );

//This function will render text for us...
void PrintText( std::string fontName, std::string text, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB );
void PrintText( std::string text, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB );

int main( int argc, char** argv )
{
	if( !Init( ) )
	{
		Quit( );
	}

	while( true )
	{
		if( ( timer - SDL_GetTicks( ) ) > FRAME_RATE )
		{
			DoThis( );
			HandleEvents( );

			timer = SDL_GetTicks( );
		}
	}

	Quit( );
	return( 0 );
}

bool Init( )
{
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
	{
		return( false );
	}
	
	// Set the caption and icon...
	SDL_WM_SetCaption( "Power Breakout", NULL );
	SDL_Surface* icon = IMG_Load( "PBdata/icon.png" );
	SDL_WM_SetIcon( icon, NULL );
	SDL_FreeSurface( icon );

	screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
	if( screen == NULL )
	{
		return( false );
	}

	// Construct paddle and ball
	paddle = new Entity( PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_COLOR, ((WINDOW_WIDTH / 2)-(PADDLE_WIDTH / 2)), (WINDOW_HEIGHT - PADDLE_HEIGHT - 16), true );
	ball = new Entity( BALL_SIZE, BALL_SIZE, BALL_COLOR, ((WINDOW_WIDTH / 2)-(BALL_SIZE / 2)), ((WINDOW_HEIGHT - PADDLE_HEIGHT - BALL_SIZE - 16 )), true );
	level = new Level( );

	SDL_Surface* temp;

	temp = IMG_Load( "PBdata/blocks.png" );
	SDL_SetColorKey( temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB( temp->format, 0, 0, 255 ) );
	blocksS = SDL_DisplayFormat( temp );
	SDL_FreeSurface( temp );

	temp = IMG_Load( "PBdata/paddle.png" );
	SDL_SetColorKey( temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB( temp->format, 0, 0, 255 ) );
	paddleS = SDL_DisplayFormat( temp );
	SDL_FreeSurface( temp );

	temp = IMG_Load( "PBdata/ball.png" );
	SDL_SetColorKey( temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB( temp->format, 0, 0, 255 ) );
	ballS = SDL_DisplayFormat( temp );
	SDL_FreeSurface( temp );

	temp = IMG_Load( "PBdata/bomb.png" );
	SDL_SetColorKey( temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB( temp->format, 0, 0, 255 ) );
	bombS = SDL_DisplayFormat( temp );
	SDL_FreeSurface( temp );

	TTF_Init( );

	arial12 = TTF_OpenFont( "PBdata/arial.ttf", 12 );

	timer = SDL_GetTicks( );

	DoThis = Intro;
	HandleEvents = HandleIntro;

	return( true );
}

// ReInit allows us to play multiple games w/o having to reload everything
void ReInit( )
{
	//Turn off the cursor and prevent it from leaving
	SDL_WM_GrabInput( SDL_GRAB_ON );
	SDL_ShowCursor( SDL_DISABLE );

	level->Load( 1 );

	moveBallWithPaddle	=	true;

	won					=	false;
	lost				=	false;
	wonGame				=	false;
	lostGame			=	false;

	score =	0;

	levelNum			=	1;

	lives				=	4;
	blasts				=	3;
	pauses				=	3;
	reverses			=	3;

	paused				=	false;
	blasted				=	false;
	reversed			=	false;

	ball->SetPosition(((WINDOW_WIDTH / 2)-(BALL_SIZE / 2)), ((WINDOW_HEIGHT - PADDLE_HEIGHT - BALL_SIZE - 16 )));
	ball->SetMovement( 0, 0 );

	paddle->SetPosition( ((WINDOW_WIDTH / 2)-(PADDLE_WIDTH / 2)), (WINDOW_HEIGHT - PADDLE_HEIGHT - 16) );

	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );	//Clear Screen to Black
	PrintText( "PBdata/adlib.ttf", "Get Ready for Level 1", 220, 200, 18, 255, 255, 255, 0, 0, 0 );
	SDL_Flip( screen );
	SDL_Delay( 2000 );

}

void Quit( )
{
	SDL_FreeSurface( blocksS );
	SDL_FreeSurface( ballS );
	SDL_FreeSurface( paddleS );
	SDL_FreeSurface( bombS );

	TTF_CloseFont( arial12 );

	SDL_Quit( );
	TTF_Quit( );

	exit( 0 );
}






void Intro( )
{
	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 255, 255, 255 ) );	//Clear screen to white

	SDL_Surface* logo = IMG_Load( "PBdata/logo.png" );


	SDL_Rect dest;
	dest.x = 60;
	dest.y = 80;
	dest.w = logo->w;
	dest.h = logo->h;

	SDL_BlitSurface( logo, NULL, screen, &dest );
	
	PrintText( "PBdata/arial.ttf", "http://www.angelfire.com/droid/simian", 255, 265, 14, 0, 0, 0, 255, 255, 255 );
	PrintText( "PBdata/adlib.ttf", "Presents...", 250, 295, 28, 0, 0, 0, 255, 255, 255 );


	SDL_Flip( screen );
	SDL_Delay( 2000 );
	SDL_FreeSurface( logo );

	//We load a level called 0 for the menu...
	level->Load( 0 );
	ball->SetPosition( 300, 400 );
	ball->SetMovement( 150, -150 );

	DoThis = Menu;
	HandleEvents = HandleMenu;
}

void HandleIntro( )
{
	//We don't actually do anything...
}

void Menu( )
{
	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );

	ball->Update( );
	if( level->CollideWithBlocks( ball ) )
	{
		ball->MultMovement( 1, -1 );
	}
	if( (ball->GetPosit().x <= 0) || (ball->GetPosit().x >=620 ) )
	{
		ball->MultMovement( -1, 1 );
	}
	if( (ball->GetPosit().y <= 0) || (ball->GetPosit().y >= 460 ) )
	{
		ball->MultMovement( 1, -1 );
	}

	level->Draw( blocksS, &screen );

	PrintText( "PBdata/adlib.ttf", "Power Breakout", 240, 50, 20, 255, 255, 255, 0, 0, 0 );

	PrintText( "PBdata/arial.ttf", "1. Play the Game!", 100, 150, 18, 255, 255, 255, 0, 0, 0 );
	PrintText( "PBdata/arial.ttf", "2. Help", 100, 180, 18, 255, 255, 255, 0, 0, 0 );
	PrintText( "PBdata/arial.ttf", "3. Quit", 100, 210, 18, 255, 255, 255, 0, 0, 0 );

	ball->Draw( ballS, &screen );

	SDL_Flip( screen );
}

void HandleMenu( )
{
	if( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT )
		{
			Quit( );			
		}
		else if( event.type == SDL_KEYDOWN )
		{
			if( event.key.keysym.sym == SDLK_ESCAPE )
			{
				Quit( );
			}
			else if( event.key.keysym.sym == SDLK_1 )
			{
				ReInit( );
				DoThis = Game;
				HandleEvents = HandleGame;
			}
			else if( event.key.keysym.sym == SDLK_2 )
			{
				DoThis = Help;
				HandleEvents = HandleHelp;
			}
			else if( event.key.keysym.sym == SDLK_3 )
			{
				Quit( );
			}
		}
	}

}

void Help( )
{
	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );

	ball->Update( );
	if( level->CollideWithBlocks( ball ) )
	{
		ball->MultMovement( 1, -1 );
	}
	if( (ball->GetPosit().x <= 0) || (ball->GetPosit().x >=620 ) )
	{
		ball->MultMovement( -1, 1 );
	}
	if( (ball->GetPosit().y <= 0) || (ball->GetPosit().y >= 460 ) )
	{
		ball->MultMovement( 1, -1 );
	}

	level->Draw( blocksS, &screen );

	PrintText( "PBdata/adlib.ttf", "Power Breakout", 240, 50, 20, 255, 255, 255, 0, 0, 0 );

	PrintText( "Help:"                                    , 75,  90, 255, 255, 255, 0, 0, 0 );
	PrintText( "There are three powers in Power Breakout:", 75, 120, 255, 255, 255, 0, 0, 0 );
	PrintText( "Left Mouse Button: Pause"                 , 75, 140, 255, 255, 255, 0, 0, 0 );
	PrintText( "Middle Mouse Button: Reverse"             , 75, 160, 255, 255, 255, 0, 0, 0 );
	PrintText( "Right Mouse Button: Explode the Ball"     , 75, 180, 255, 255, 255, 0, 0, 0 );

	PrintText( "Use the mouse to control the paddle - Don't let the ball pass you!", 75, 220, 255, 255, 255, 0, 0, 0 );
	PrintText( "Prees \'p\' to pause the game or escape to quit at any time"       , 75, 240, 255, 255, 255, 0, 0, 0 );
	PrintText( "Copyright Ian Finlayson 2005 - Released under the GNU general public license", 75, 260, 255, 255, 255, 0, 0, 0 );
	PrintText( "Art by Caitie Cihak"                                                         , 75, 280, 255, 255, 255, 0, 0, 0 );

	PrintText( "Press Enter to return to Menu", 75, 330, 255, 255, 255, 0, 0, 0 );

	ball->Draw( ballS, &screen );
	SDL_Flip( screen );
}

void HandleHelp( )
{
	if( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT )
		{
			Quit( );
		}
		else if( event.type == SDL_KEYDOWN )
		{
			if( ( event.key.keysym.sym == SDLK_ESCAPE ) || ( event.key.keysym.sym == SDLK_RETURN ) )
			{
				DoThis = Menu;
				HandleEvents = HandleMenu;
			}
		}
	}
}

void Game( )
{
	if( gameIsPaused )	//If the game is paused, we don't want anything to happen
	{
		return;
	}

	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );	//Clear Screen to Black

	if( level->LevelComplete( ) )
	{
		if( ( levelNum ) == NUMBER_OF_LEVELS )
		{
			wonGame = true;
		}
		else
		{
			won = true;
		}
	}

	if( ( ball->GetPosit( ).y + ball->GetPosit().h ) > ( WINDOW_HEIGHT + 32 ) )
	{
		if( ( lives - 1 ) == 0 )
		{
			lostGame = true;
		}
		else
		{
			lost = true;
		}
	}

	//Move the Ball
	ball->Update( );
	
	int deletions = level->CollideWithBlocks( ball );
	score += ( deletions * 10 );
	//If ball hits a block, Set it Moving back...
	if( deletions > 0 )
	{
		ball->MultMovement( 1, -1 );
		ball->Update( );
	}


	//If ball collides with Paddle, move it back...
	if( ball->Collide( paddle->GetPosit( ) ) && ( !moveBallWithPaddle ) )
	{
		//percent represents the percentage of way ball is across paddle...
		float percent = (float)( (float)( (ball->GetPosit().x+(ball->GetPosit().w/2)) - paddle->GetPosit().x ) / (float)paddle->GetPosit().w );

		//We change the horizontal velocity of the ball based on position along the paddle
		//The paddle is divided into 10 regions
		if( percent < 0.000f )	// Try Different Values here...
		{
			ball->AddToMovement( -50.0f, 0 );
		}
		else if( percent < 0.125f )
		{
			ball->AddToMovement( -35.35f, 0 );
		}
		else if( percent < 0.250f )
		{
			ball->AddToMovement( -20.0f, 0 );
		}
		else if( percent < 0.375f )
		{
			ball->AddToMovement( -10.6f, 0 );
		}
		else if( percent < 0.500f )
		{
			ball->AddToMovement( -5.25f, 0 );
		}
		else if( percent < 0.625f )
		{
			ball->AddToMovement( 5.35f, 0 );
		}
		else if( percent < 0.750f )
		{
			ball->AddToMovement( 10.5f, 0 );
		}
		else if( percent < 0.875f )
		{
			ball->AddToMovement( 20.1f, 0 );
		}
		else if( percent < 1.000f )
		{
			ball->AddToMovement( 35.25f, 0 );
		}
		else
		{
			ball->AddToMovement( 500.0f, 0 );
		}
		if( ball->GetPosit().x < 0 )
		{
			ball->Move( 30, 0 );
		}

		//If the ball is going to fast horizontally, set it back
		if( ball->GetXSpeed( ) > MAX_HORIZ_SPEED )
		{
			ball->SetMovement( MAX_HORIZ_SPEED, ball->GetYSpeed( ) );
		}

		if( ball->GetXSpeed( ) < -MAX_HORIZ_SPEED )
		{
			ball->SetMovement( -MAX_HORIZ_SPEED, ball->GetYSpeed( ) );
		}

		ball->MultMovement( 1, -1 );
		ball->Update( );
	}

	//If ball hits ceiling, move back...
	if( ball->GetPosit( ).y < 0 )
	{
		ball->MultMovement( 1, -1 );
		ball->Update( );
	}
	//if ball hits wall...
	if( ( ( ball->GetPosit( ).x + ball->GetPosit( ).w) > WINDOW_WIDTH ) || (ball->GetPosit( ).x < 0 ) )
	{
		ball->MultMovement( -1, 1 );
	}

	level->Draw( blocksS, &screen );
	ball->Draw( ballS, &screen );
	paddle->Draw( paddleS, &screen );

	std::stringstream scoreStr, levelStr, livesStr, pauseStr, bombStr, revStr;

	scoreStr << "Score:  " << score;
	levelStr << "Level:  " << levelNum;
	livesStr << "Lives:  " << lives - 1;
	pauseStr << "Pauses: " << pauses;
	bombStr  << "Blasts: " << blasts;
	revStr   << "Reverses: " << reverses;

	PrintText( scoreStr.str(),   0, 0, 255, 255, 255, 0, 0, 0 );
	PrintText( levelStr.str(),  75, 0, 255, 255, 255, 0, 0, 0 );
	PrintText( livesStr.str(), 130, 0, 255, 255, 255, 0, 0, 0 );

	if( moveBallWithPaddle )
	{
		PrintText( "Press Any Mouse Button to Launch", 215, 0, 255, 255, 255, 0, 0, 0 );
	}

	PrintText( pauseStr.str(), 430, 0, 255, 255, 255, 0, 0, 0 );
	PrintText( revStr.str(),   490, 0, 255, 255, 255, 0, 0, 0 );
	PrintText( bombStr.str(),  570, 0, 255, 255, 255, 0, 0, 0 );

	SDL_Flip( screen );
}

void HandleGame( )
{
	if( lost && !gameIsPaused )
	{
		lives--;

		paddle->SetPosition(((WINDOW_WIDTH / 2)-(PADDLE_WIDTH / 2)), (WINDOW_HEIGHT - PADDLE_HEIGHT - 16));
		ball->SetPosition(((WINDOW_WIDTH / 2)-(BALL_SIZE / 2)), ((WINDOW_HEIGHT - PADDLE_HEIGHT - BALL_SIZE - 16 )));
		ball->SetMovement( 0, 0 );

		moveBallWithPaddle = true;
		lost = false;
	}

	if( won && !gameIsPaused )
	{
		blasts = 3;
		pauses = 3;
		reverses = 3;

		lives++;

		levelNum++;

		std::stringstream levelStr;
		levelStr << "Get Ready for Level " << levelNum;
		SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );	//Clear Screen to Black
		PrintText( "PBdata/adlib.ttf", levelStr.str(), 220, 200, 18, 255, 255, 255, 0, 0, 0 );
		SDL_Flip( screen );
		SDL_Delay( 2000 );



		level->Load( levelNum );

		paddle->SetPosition(((WINDOW_WIDTH / 2)-(PADDLE_WIDTH / 2)), (WINDOW_HEIGHT - PADDLE_HEIGHT - 16));
		ball->SetPosition(((WINDOW_WIDTH / 2)-(BALL_SIZE / 2)), ((WINDOW_HEIGHT - PADDLE_HEIGHT - BALL_SIZE - 16 )));
		ball->SetMovement( 0, 0 );

		moveBallWithPaddle = true;

		won = false;
	}

	if( wonGame && !gameIsPaused )
	{
		level->Load( 0 );
		ball->SetPosition( 300, 400 );
		ball->SetMovement( 150, -150 );
		
		DoThis = Won;
		HandleEvents = HandleWon;
	}

	if( lostGame && !gameIsPaused )
	{
		level->Load( 0 );
		ball->SetPosition( 300, 400 );
		ball->SetMovement( 150, -150 );
		
		DoThis = Lost;
		HandleEvents = HandleLost;
	}

	//This chunk of code handles the pause power
	if( (paused && ( ( ( SDL_GetTicks( ) - pausedTimer ) < 1000 ) ) ) && !gameIsPaused )
	{
		ball->SetMovement( 0, 0 );
	}
	else if( paused && !gameIsPaused )
	{
		ball->SetMovement( oldX, oldY );
		paused = false;
	}

	if( reversed && !gameIsPaused )
	{
		reversed = false;
		ball->ReverseMovement( );
	}

	//This chunk of code interrupts the normal flow to show the user a little explosion
	if( blasted && !gameIsPaused )
	{
		blasted = false;
		
		//This creates a new Entity for the bomb. The bomb is a sprite so instead of creating
		//A sprite class for this one little situation, we'll hack our Entity class to do what we want
		Entity bomb( 64, 64, NULL, ball->GetPosit().x - 24, ball->GetPosit().y - 24, true );
		
		float oldX = ball->GetXSpeed( );
		float oldY = ball->GetYSpeed( );
		ball->SetMovement( 0, 0 );

		for( int i=0; i<4; i++ )	//We have four frames of animation
		{
			bomb.Draw( bombS, &screen );
			SDL_Flip( screen );
			SDL_Delay( 100 );

			bomb.AddToSrcRect( 64, 0 );
		}

		ball->Update( );
		ball->Update( );
		ball->SetMovement( oldX, oldY );

		//This is the samefunction we normally pass ball to
		//ballis 16x16  but bomb is 64x64 so the bomb will
		//take out more blocks causing area of damage effect
		int bombScore = level->CollideWithBlocks( &bomb );

		score += ( 10*bombScore );
	}

	if( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT )
		{
			Quit( );
		}

		else if( event.type == SDL_KEYDOWN )
		{
			if( event.key.keysym.sym == SDLK_ESCAPE )
			{
				//If they hit escape, they go back to menu
				//We load a level called 0 for the menu...
				level->Load( 0 );
				ball->SetPosition( 300, 400 );
				ball->SetMovement( 150, -150 );

				DoThis = Menu;
				HandleEvents = HandleMenu;
			}
			else if( event.key.keysym.sym == SDLK_p )
			{
				gameIsPaused = !gameIsPaused;

				if( gameIsPaused )
				{
					speed_x = ball->GetXSpeed( );
					speed_y = ball->GetYSpeed( );

					ball->SetMovement( 0, 0 );

					SDL_WM_GrabInput( SDL_GRAB_OFF );
					SDL_ShowCursor( SDL_ENABLE );
				}
				else
				{
					ball->SetMovement( speed_x, speed_y );

					SDL_WM_GrabInput( SDL_GRAB_ON );
					SDL_ShowCursor( SDL_DISABLE );
				}
			}

		}

		else if( ( event.type == SDL_MOUSEBUTTONDOWN ) && !gameIsPaused )
		{
			if( event.button.button == SDL_BUTTON_LEFT )
			{
				if( moveBallWithPaddle )
				{
					ball->SetMovement( 0, -level->GetBallSpeed( ) );
					moveBallWithPaddle = false;
				}
				else
				{
					if( (pauses > 0) && !paused )
					{
						//Player uses a "pause"
						paused = true;
						pausedTimer = SDL_GetTicks( );

						oldX = ball->GetXSpeed( );
						oldY = ball->GetYSpeed( );

						pauses--;
					}

				}

			}
			else if( event.button.button == SDL_BUTTON_RIGHT )
			{
				if( moveBallWithPaddle )
				{
					ball->SetMovement( 0, -level->GetBallSpeed( ) );
					moveBallWithPaddle = false;
				}
				else
				{
					if( blasts > 0 )
					{
						blasted = true;
						blasts--;
					}
				}
			}
			else if( event.button.button == SDL_BUTTON_MIDDLE )
			{
				if( moveBallWithPaddle )
				{
					ball->SetMovement( 0, -level->GetBallSpeed( ) );
					moveBallWithPaddle = false;
				}
				else
				{
					if( reverses > 0 )
					{
						reversed = true;
						reverses--;
					}
				}
			}
		}

		else if( ( event.type == SDL_MOUSEMOTION ) && !gameIsPaused )
		{
			int change = event.motion.xrel / PLAYER_SPEED;

			paddle->Move( change, 0 );

			//We would do this before player "fires ball off"
			if( moveBallWithPaddle )
			{
				ball->Move( change, 0 );
			}

			// Find out where paddle is now...
			SDL_Rect place = paddle->GetPosit( );

			//if paddle is off screen...
			if( ( place.x < 0 ) || ( ( place.x + place.w ) >  WINDOW_WIDTH ) )
			{
				//Move it back...
				paddle->Move( -change, 0 );
				if( moveBallWithPaddle )
				{
					ball->Move( -change, 0 );
				}
			}
		}
	}

	ball->Update( );
}

void Won( )
{
	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );

	ball->Update( );
	if( level->CollideWithBlocks( ball ) )
	{
		ball->MultMovement( 1, -1 );
	}
	if( (ball->GetPosit().x <= 0) || (ball->GetPosit().x >=620 ) )
	{
		ball->MultMovement( -1, 1 );
	}
	if( (ball->GetPosit().y <= 0) || (ball->GetPosit().y >= 460 ) )
	{
		ball->MultMovement( 1, -1 );
	}

	level->Draw( blocksS, &screen );

	PrintText( "PBdata/adlib.ttf", "Power Breakout", 240, 50, 20, 255, 255, 255, 0, 0, 0 );

	PrintText( "PBdata/adlib.ttf", "Congratsulations! You Won!", 120, 150, 20, 255, 255, 255, 0, 0, 0 );
	PrintText( "PBdata/arial.ttf", "Press Enter to Play Again or Escape to Quit", 120, 200, 16, 255, 255, 255, 0, 0, 0 );


	ball->Draw( ballS, &screen );

	SDL_Flip( screen );
}

void HandleWon( )
{
	if( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT )
		{
			Quit( );
		}
		else if( event.type == SDL_KEYDOWN )
		{
			if( event.key.keysym.sym == SDLK_RETURN )
			{
				DoThis = Game;
				HandleEvents = HandleGame;

				ReInit( );
			}
			else if( event.key.keysym.sym == SDLK_ESCAPE )
			{
				Quit( );
			}
		}
	}
}

void Lost( )
{
	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );

	ball->Update( );
	if( level->CollideWithBlocks( ball ) )
	{
		ball->MultMovement( 1, -1 );
	}
	if( (ball->GetPosit().x <= 0) || (ball->GetPosit().x >=620 ) )
	{
		ball->MultMovement( -1, 1 );
	}
	if( (ball->GetPosit().y <= 0) || (ball->GetPosit().y >= 460 ) )
	{
		ball->MultMovement( 1, -1 );
	}

	level->Draw( blocksS, &screen );

	PrintText( "PBdata/adlib.ttf", "Power Breakout", 240, 50, 20, 255, 255, 255, 0, 0, 0 );

	PrintText( "PBdata/adlib.ttf", "Better Luck Next Time Sucker!", 120, 150, 20, 255, 255, 255, 0, 0, 0 );
	PrintText( "PBdata/arial.ttf", "Press Enter to Play Again or Escape to Quit", 120, 200, 16, 255, 255, 255, 0, 0, 0 );

	ball->Draw( ballS, &screen );

	SDL_Flip( screen );
}

void HandleLost( )
{
	if( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT )
		{
			Quit( );
		}
		else if( event.type == SDL_KEYDOWN )
		{
			if( event.key.keysym.sym == SDLK_RETURN )
			{
				DoThis = Game;
				HandleEvents = HandleGame;

				ReInit( );
			}
			else if( event.key.keysym.sym == SDLK_ESCAPE )
			{
				Quit( );
			}
		}
	}
}

void PrintText( std::string fontName, std::string text, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB )
{
	TTF_Font* font = TTF_OpenFont( fontName.c_str(), size );

	SDL_Color fore = { fR, fG, fB };
	SDL_Color back = { bR, bG, bB };

	SDL_Surface* temp = TTF_RenderText_Shaded( font, text.c_str(), fore, back );

	SDL_Rect dest = { x, y, 0, 0 };

	SDL_BlitSurface( temp, NULL, screen, &dest );

	SDL_FreeSurface( temp );


	TTF_CloseFont( font );
}

//This print text does not have to load fonts (it only works for arial 12)
void PrintText( std::string text, int x, int y, int fR, int fG, int fB, int bR, int bG, int bB )
{
	SDL_Color fore = { fR, fG, fB };
	SDL_Color back = { bR, bG, bB };

	SDL_Surface* temp = TTF_RenderText_Shaded( arial12, text.c_str(), fore, back );

	SDL_Rect dest = { x, y, 0, 0 };

	SDL_BlitSurface( temp, NULL, screen, &dest );

	SDL_FreeSurface( temp );
}