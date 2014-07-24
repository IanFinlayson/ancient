// Main.cpp		-	Ties everything together...
// Ian Finlayson

// 3D Pong
// Move the paddle with the arrow keys. Use the 'o' key to move forwards, and the 'l' key to move backwards.
// Try to keep the ball from going past you!

#include <cstdio>	// For sprintf

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Constants.h"
#include "Window.h"
#include "Timer.h"
#include "TextWriter.h"
#include "Ball.h"
#include "Paddle.h"
#include "resource.h"

// Global variables...
// These are set in the dialog box...
bool FULL_SCREEN;
bool SOUND_ON;

float BALL_RADIUS;
float BALL_SPEED;

float PADDLE_WIDTH;
float PADDLE_HEIGHT;
float PADDLE_SPEED;

int bounces;
int losses;

// Functions...
void Init( );
bool Update( Timer & timer, Ball & ball, Paddle & paddle );
void Draw( Ball & ball, Paddle & paddle, TextWriter & tw );


// Declares the message handeler for the dialog box
LRESULT CALLBACK DlgProc( HWND, UINT, WPARAM,  LPARAM );
HWND hwnd;		// The handle to the dialog box


int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prev, LPSTR cmdline, int nCmdShow )
{
	// Do a dialog box to get options
	DialogBox( hinst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC) DlgProc );

	Window window( static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ), FULL_SCREEN, 32, "3D Pong!" );

	Timer timer;
	Paddle paddle;
	Ball ball( &paddle );
	TextWriter tw( &window );

	Init( );

	MSG msg;
	bool done = false;
	bool game_over = false;


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

		if( !Update( timer, ball, paddle ) )	// The user lost...
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
					int score = bounces - 2*losses;
					if( score < 0 )
					{
						MessageBox( NULL, "Negative Score? You are pathetic!", "Pong", NULL );
					}
					else if( score < 10 )
					{
						MessageBox( NULL, "You did fair.", "Pong", NULL );
					}
					else if( score < 50 )
					{
						MessageBox( NULL, "You did awesome", "Pong", NULL );
					}
					else
					{
						if( BALL_RADIUS < BALL_RADIUS_EASY )
						{
							MessageBox( NULL, "You are a pong God!", "Pong", NULL );
						}
						else
						{
							MessageBox( NULL, "You did great. Maybe next time you'll pick a harder setting...", "Pong", NULL );
						}
					}

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

		Draw( ball, paddle, tw );
		window.Swap( );
	}

	return 0;
}

void Draw( Ball & ball, Paddle & paddle, TextWriter & tw )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Draw the ball
	ball.Draw( );	

	// Draw the border lines
	glColor4f( 0.5f, 1.0f, 0.5f, 1.0f );
	glBegin( GL_LINES );
		glVertex3f( -WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, FRONT_Z );
		glVertex3f( -WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, BACK_Z );

		glVertex3f( WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, FRONT_Z );
		glVertex3f( WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, BACK_Z );		

		glVertex3f( -WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, FRONT_Z );
		glVertex3f( -WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, BACK_Z );

		glVertex3f( WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, FRONT_Z );
		glVertex3f( WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, BACK_Z );
	glEnd( );

	// Draw the perspective lines...
	for( float z=FRONT_Z; z>=BACK_Z; z-=115.0f )
	{
		glBegin( GL_LINE_LOOP );
			glVertex3f( -WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, z );
			glVertex3f( -WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, z );
			glVertex3f( WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, z );
			glVertex3f( WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, z );
		glEnd( );
	}

	// Draw a quad as the back wall...
	glBegin( GL_QUADS );
		glVertex3f( -WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, BACK_Z );
		glVertex3f( -WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, BACK_Z );
		glVertex3f( WINDOW_WIDTH/2.0f, -WINDOW_HEIGHT/2.0f, BACK_Z );
		glVertex3f( WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, BACK_Z );
	glEnd( );

	// Draw the paddle
	paddle.Draw( );

	// Temporary string buffer
	// It's static so the space is only
	// allocated once...
	static char temp[31];

	// Write the text...
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	sprintf( temp, "Bounces: %i", bounces );
	tw.WriteText( -0.460f, 0.365f, temp );

	sprintf( temp, "Losses: %i", losses );
	tw.WriteText( -0.220f, 0.365f, temp );

	sprintf( temp, "Score: %i", bounces - 2*losses );
	tw.WriteText( 0.225f, 0.365f, temp );
}

void Init( )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth( 1.0f );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

bool Update( Timer & timer, Ball & ball, Paddle & paddle )
{
	timer.Update( );

	float time = timer.GetElapsedTime( );

	paddle.Update( time );

	return ball.Update( time );
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
		// Cheack the medium button for default
		CheckRadioButton( h, IDC_SIMPLE, IDC_INSANE, IDC_MEDIUM );

		return true;
	}
	else if( msg == WM_CLOSE )
	{
		exit( 0 );
	}
	else if( msg == WM_COMMAND )
	{
		if( w == ID_QUIT )
		{
			exit( 0 );
		}
		else if( w == ID_START )
		{
			// Check and see what difficulty level they wanted
			// And assign the necesary values...
			if( IsDlgButtonChecked( h, IDC_SIMPLE ) )
			{
				BALL_RADIUS = BALL_RADIUS_SIMPLE;
				BALL_SPEED = BALL_SPEED_SIMPLE;
				PADDLE_WIDTH = PADDLE_SIZE_SIMPLE;
				PADDLE_HEIGHT = PADDLE_SIZE_SIMPLE;
				PADDLE_SPEED = PADDLE_SPEED_SIMPLE;
			}
			else if( IsDlgButtonChecked( h, IDC_EASY ) )
			{
				BALL_RADIUS = BALL_RADIUS_EASY;
				BALL_SPEED = BALL_SPEED_EASY;
				PADDLE_WIDTH = PADDLE_SIZE_EASY;
				PADDLE_HEIGHT = PADDLE_SIZE_EASY;
				PADDLE_SPEED = PADDLE_SPEED_EASY;
			}
			else if( IsDlgButtonChecked( h, IDC_MEDIUM ) )
			{
				BALL_RADIUS = BALL_RADIUS_MEDIUM;
				BALL_SPEED = BALL_SPEED_MEDIUM;
				PADDLE_WIDTH = PADDLE_SIZE_MEDIUM;
				PADDLE_HEIGHT = PADDLE_SIZE_MEDIUM;
				PADDLE_SPEED = PADDLE_SPEED_MEDIUM;
			}
			else if( IsDlgButtonChecked( h, IDC_HARD ) )
			{
				BALL_RADIUS = BALL_RADIUS_HARD;
				BALL_SPEED = BALL_SPEED_HARD;
				PADDLE_WIDTH = PADDLE_SIZE_HARD;
				PADDLE_HEIGHT = PADDLE_SIZE_HARD;
				PADDLE_SPEED = PADDLE_SPEED_HARD;
			}
			else	// Insane!
			{
				BALL_RADIUS = BALL_RADIUS_INSANE;
				BALL_SPEED = BALL_SPEED_INSANE;
				PADDLE_WIDTH = PADDLE_SIZE_INSANE;
				PADDLE_HEIGHT = PADDLE_SIZE_INSANE;
				PADDLE_SPEED = PADDLE_SPEED_INSANE;
			}

			// See if they wanted to play in fullscreen mode
			FULL_SCREEN = IsDlgButtonChecked( h, IDC_FULLSCREEN )? true : false;
			SOUND_ON = IsDlgButtonChecked( h, IDC_SOUND )? true : false;
			

			// Because they pushed start, we end the dialog box and get on with the game!
			EndDialog( h, true );
		}

		// Messagee handled
		return true;
	}

	// Message not handeled
	return false;
}

