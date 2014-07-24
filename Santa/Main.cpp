// Main.cpp		-	Ties everything together...
// Ian Finlayson

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Constants.h"
#include "Window.h"
#include "Timer.h"

#include "BmpTexture.h"
#include "SkyBox.h"
#include "Man.h"
#include "Model.h"
#include "SnowStorm.h"

void Draw( SkyBox & skybox, Man & man, Model & tree, BmpTexture & tree_tex, SnowStorm & snow_storm );
void Update( Timer & timer, Man & man, SnowStorm & snow_storm );

// Used for input
inline bool KeyDown( char vk_code )
{
	return ( (GetAsyncKeyState( vk_code ) & 0x8000)? true : false );
}

inline bool KeyUp( char vk_code )
{
	return !KeyDown( vk_code );
}


// Global rotation variables (for moving camera)
float roty = 0.0f;
float rotx = 0.0f;
float in = -138.0f;		// How far away we are from the tree...

// Global Window Object (so WndProc can have access to it)
Window window( WINDOW_WIDTH, WINDOW_HEIGHT, 32, "Walking Man" );

// Main...
int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prev, LPSTR cmdline, int nCmdShow )
{
	MSG msg;
	bool done = false;

	// Turn on depth testing...
	glEnable( GL_DEPTH_TEST );

	// Our objects...
	SkyBox skybox( "images/1.bmp", "images/2.bmp", "images/3.bmp", "images/4.bmp", "images/5.bmp", "images/6.bmp" );
	Man man;
	Timer timer;
	Model tree( "models/tree.ms3d" );
	BmpTexture tree_tex( "images/tree.bmp" );
	SnowStorm snow_storm;

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

		Update( timer, man, snow_storm );
		Draw( skybox, man, tree, tree_tex, snow_storm );
		window.Swap( );
	}

	return 0;
}

// Update game elements....
void Update( Timer & timer, Man & man, SnowStorm & snow_storm )
{
	// Get Input...
	if( KeyDown( VK_UP ) )
		rotx -= .4f;
	else if( KeyDown( VK_DOWN ) )
		rotx += .4f;

	if( KeyDown( VK_LEFT ) )
		roty -= .4f;
	else if( KeyDown( VK_RIGHT ) )
		roty += .4f;

	if( KeyDown( 'O' ) )
		in += .4f;
	else if( KeyDown( 'L' ) )
		in -= .4f;

	// Update game elements...
	timer.Update( );
	float elapsed_time = timer.GetElapsedTime( );

	man.Update( elapsed_time );
	snow_storm.Update( elapsed_time );
}

// Draw everything...
void Draw( SkyBox & skybox, Man & man, Model & tree, BmpTexture & tree_tex, SnowStorm & snow_storm )
{
	// Clear the stuff...
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity( );

	// Rotate camera
	glRotatef( roty, 0.0f, 1.0f, 0.0f );
	glRotatef( rotx, 1.0f, 0.0f, 0.0f );

	// Move camera
	glTranslatef( 0.0f, 0.0f, in );
	
	// Draw the skybox (with a bright bluish hue)
	glColor3f( 0.80f, 0.80f, 1.0f );
	skybox.Draw( );
	glColor3f( 1.0f, 1.0f, 1.0f );

	// Need to set up the rotation/translation!
	static float theta = 0.0f;
	theta += 0.25;

	// Move the tree down...
	glTranslatef( 0.0f, -25.0f, 0.0f );
	
	// Put the ugly side of the tree out of sight (just like real life)
	glRotatef( 90.0f, 0.0f, 1.0f, 0.0f );

	// Draw the tree...
	tree_tex.Bind( );
	tree.Draw( );

	// Rotate the man (to make him walk in a circle
	glRotatef( theta, 0.0f, 1.0f, 0.0f );

	// Move the man out (radius of the circle)
	glTranslatef( 0.0f, 0.0f, 75.0f );

	// Make him a little bigger...
	glScalef( 1.5f, 1.5f, 1.5f );

	// Rotate the man (so he is walking forward)
	glRotatef( 90.0f, 0.0f, 1.0f, 0.0f );

	// Draw the man...
	man.Draw( );

	// Draw the Snow on top of everything...
	snow_storm.Draw( );
}

// The Message handeler for the main window
LRESULT CALLBACK WndProc( HWND h, UINT msg, WPARAM w, LPARAM l )
{
	// All we do here is see if the user quit or pushed escape and exit...
	// Also resize the scene...
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
	else if( msg == WM_SIZE )
	{
		window.Resize( LOWORD(l),HIWORD(l) );
	}

	return DefWindowProc( h, msg, w, l );
}
