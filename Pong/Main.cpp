// Main.cpp
// Ian Finlayson

// This program is a pong game. There are 3 walls: top, bottom, and left. There is a paddle on the right
// The user controls the paddle with the up and down keys.  There are three difficulty settings: easy,
// medium and hard. The difficulty affects: Paddle speed, ball speed, ball radius, and paddle height
// Hitting the ball with the paddle causes the ball to reflect off. The part of the paddle the ball
// hits affects the angle of reflection. Once the ball eludes the paddle, the user is given the option to
// start a new game or quit.  You can also quit by pressing the close button (in window mode) or hitting escape.

// The first thing main does is give the user a dialog box so they can give their difficulty setting
// and whether or not they want to play in fullscreen. It then gets on with the game.

// Main header files
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Window.h"
#include "Timer.h"
#include "Constants.h"
#include "Ball.h"
#include "Paddle.h"
#include "resource.h"	// The resource.h files are generated by MSVC, 
#include "resource1.h"	// They just have constants for using the dialog box...

void Init( );
bool Update( Timer&, Paddle&, Ball& );
void Draw( Paddle&, Ball& );

// Definitions of the global variables...
// The values of these will be assigned in the Dialog box
// then they will be used as constants (hence the capitals)
int BALL_RADIUS;
float BALL_SPEED;

float PADDLE_SPEED;
int PADDLE_HEIGHT;

bool FULL_SCREEN;

// Declares the message handeler for the dialog box
LRESULT CALLBACK DlgProc( HWND, UINT, WPARAM,  LPARAM );
HWND hwnd;		// The handle to the dialog box

int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prev, LPSTR cmdline, int nCmdShow )
{
	// Do a dialog box to get options
	DialogBox( hinst, MAKEINTRESOURCE(IDD_DIALOG), hwnd, (DLGPROC) DlgProc );

	// Instantiate our objects...
	Window window( WINDOW_WIDTH, WINDOW_HEIGHT, FULL_SCREEN, 32, "Pong!" );
	Timer timer;
	Paddle paddle;
	Ball ball( &paddle );

	Init( );

	MSG msg;
	bool done = false;
	bool game_over = false;		// The game over variable is to prevent the MessageBox from appearing twice

	while( !done )
	{
		// Check for messages...
		if( PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) )
		{
			if( msg.message == WM_QUIT )
			{
				done = true;
			}			
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// If update returns false, the user lost...
		if( !Update( timer, paddle, ball ) )	// If game over
		{
			// The game over variable is usedto keep the "Lost" Message popping up twice in a row...
			if( !game_over )
			{
				if( FULL_SCREEN )	// If the cursor is hidden, we must bring it back...
				{
					ShowCursor( true );
				}

				// We ask the user if they want another game...
				if( MessageBox( NULL, "Sorry You Lost\nTry Again?", "Pong", MB_YESNO | MB_ICONQUESTION ) == IDNO )
				{
					// If they say no
					PostQuitMessage( 0 );
					game_over = true;
				}
				else	// They said yes...
				{
					// Reposition the Ball
					ball.Reset( );

					// We must call timer.Update( ) to clear out the delay caused by the message box...
					// --Without doing this, it will think 3 seconds have passed, and the ball will travel warp speed across the screen--
					timer.Update( );

					if( FULL_SCREEN )	// Re-hide the cursor
					{
						ShowCursor( false );
					}
				}
			}
		}

		// Draw everything...
		Draw( paddle, ball );
		window.Swap( );
	}

	return 0;
}

// Initialize OpenGL
void Init( )
{
	// Set the clear color to black
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClearDepth( 1.0 );
}

// Draw the scene
void Draw( Paddle& paddle, Ball& ball )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// We draw the walls here
	glBegin( GL_QUADS );

		// The bottom wall
		glVertex2i( 0, 0 );
		glVertex2i( WINDOW_WIDTH, 0 );
		glVertex2i( WINDOW_WIDTH, WALL_WIDTH );
		glVertex2i( 0, WALL_WIDTH );
		
		// The left wall
		glVertex2i( 0, WALL_WIDTH );
		glVertex2i( WALL_WIDTH, WALL_WIDTH );
		glVertex2i( WALL_WIDTH, WINDOW_HEIGHT );
		glVertex2i( 0, WINDOW_HEIGHT );

		// The top wall
		glVertex2i( 0, WINDOW_HEIGHT );
		glVertex2i( 0, WINDOW_HEIGHT - WALL_WIDTH  );
		glVertex2i( WINDOW_WIDTH, WINDOW_HEIGHT - WALL_WIDTH );
		glVertex2i( WINDOW_WIDTH, WINDOW_HEIGHT );

	glEnd( );

	// Draw the ball and paddle
	ball.Draw( );
	paddle.Draw( );
}

// This function simply calls the update methods for the timer, ball, and paddle...
bool Update( Timer& timer, Paddle& paddle, Ball& ball )
{
	timer.Update( );

	paddle.Update( timer.GetElapsedTime( ) );

	// If the ball returns false, the user lost
	if( !ball.Update( timer.GetElapsedTime( ) ) )
	{
		return false;
	}

	return true;
}


// The Message handeler for the main window
LRESULT CALLBACK WndProc( HWND h, UINT msg, WPARAM w, LPARAM l )
{
	// All we do here is see if the user quit or pushed escape and exit...
	if( msg == WM_CLOSE )
	{
		PostQuitMessage( 0 );
	}
	else if( msg == WM_KEYDOWN )
	{
		if( w == VK_ESCAPE )
		{
			PostQuitMessage( 0 );
		}
	}

	return DefWindowProc( h, msg, w, l );
}

// The message handeler for the dialog box
LRESULT CALLBACK DlgProc( HWND h, UINT msg, WPARAM w,  LPARAM l )
{
	if( msg == WM_INITDIALOG )
	{
		// Cheack the easy button for default
		CheckRadioButton( h, IDC_EASY, IDC_HARD, IDC_EASY );

		return true;
	}
	else if( msg == WM_CLOSE )
	{
		exit( 0 );
	}
	else if( msg == WM_COMMAND )
	{
		if( w == IDQUIT )
		{
			exit( 0 );
		}
		else if( w == IDSTART )
		{
			// Check and see what difficulty level they wanted
			// And assign the necesary values...
			if( IsDlgButtonChecked( h, IDC_EASY ) )
			{
				BALL_RADIUS = BALL_RADIUS_EASY;
				BALL_SPEED = BALL_SPEED_EASY;
				PADDLE_SPEED = PADDLE_SPEED_EASY;
				PADDLE_HEIGHT = PADDLE_HEIGHT_EASY;
			}
			else if( IsDlgButtonChecked( h, IDC_MEDIUM ) )
			{
				BALL_RADIUS = BALL_RADIUS_MEDIUM;
				BALL_SPEED = BALL_SPEED_MEDIUM;
				PADDLE_SPEED = PADDLE_SPEED_MEDIUM;
				PADDLE_HEIGHT = PADDLE_HEIGHT_MEDIUM;				
			}
			else	// The hard setting...
			{
				BALL_RADIUS = BALL_RADIUS_HARD;
				BALL_SPEED = BALL_SPEED_HARD;
				PADDLE_SPEED = PADDLE_SPEED_HARD;
				PADDLE_HEIGHT = PADDLE_HEIGHT_HARD;				
			}

			// See if they wanted to play in fullscreen mode
			FULL_SCREEN = IsDlgButtonChecked( h, IDC_FULLSCREEN )? true : false;
			

			// Because they pushed start, we end the dialog box and get on with the game!
			EndDialog( h, true );
		}

		// Messagee handled
		return true;
	}

	// Message not handeled
	return false;
}



