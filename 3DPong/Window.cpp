// Window.cpp
// Ian Finlayson
// Defines the OpenGL window...

#include "Window.h"

// Constructor
Window::Window( int width, int height, bool full, int bpp, const char* title )
{
	// Store whether wer'e in fullscreen
	fullscreen = full;

	// Get the hinstance thingy
	hinst = GetModuleHandle( NULL );

	// Fill in a window object
	WNDCLASSEX window;

	window.cbClsExtra		= 0;
	window.cbSize			= sizeof WNDCLASSEX;
	window.cbWndExtra		= 0;
	window.hbrBackground	= NULL;
	window.hCursor			= LoadCursor( NULL, IDC_ARROW );
	window.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	window.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );
	window.hInstance		= GetModuleHandle( NULL );
	window.lpfnWndProc		= WndProc;
	window.lpszClassName	= "OpenGl";
	window.lpszMenuName		= NULL;
	window.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	// Register it
	RegisterClassEx( &window );

	// The window rect
	RECT rect;
	rect.top    = 0;
	rect.left   = 0;
	rect.bottom = height;
	rect.right  = width;

	// For fullscreen only
	if( full )
	{
		DEVMODE devmode;
		memset( &devmode, 0, sizeof DEVMODE );

		devmode.dmSize			= sizeof DEVMODE;
		devmode.dmPelsWidth		= width;
		devmode.dmPelsHeight	= height;
		devmode.dmBitsPerPel	= bpp;
		devmode.dmFields		= DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		if( ChangeDisplaySettings( &devmode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		{
			full = false;
		}
	}

	DWORD window_style, ex_window_style;

	if( full )
	{
		window_style = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		ex_window_style = WS_EX_APPWINDOW;

		ShowCursor( false );
	}
	else
	{
		window_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		ex_window_style = NULL;
	}

	// Adjust for the title etc.
	AdjustWindowRectEx( &rect, window_style, false, ex_window_style );

	// Create the window
	hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, "OpenGl", title, 
		window_style, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hinst, NULL );

	PIXELFORMATDESCRIPTOR pfd;
	// Clear the pfd to 0
	memset( &pfd, 0, sizeof pfd );

	// Fill in the pertinent fields
	pfd.nSize			= sizeof PIXELFORMATDESCRIPTOR;
	pfd.nVersion		= 1;
	pfd.dwFlags			= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType		= PFD_TYPE_RGBA;
	pfd.cColorBits		= bpp;
	pfd.cDepthBits		= 16;	// We want a depth buffer
	pfd.iLayerType		= PFD_MAIN_PLANE;

	// Finalize all this stuff...
	hdc = GetDC( hwnd );

	SetPixelFormat( hdc, ChoosePixelFormat( hdc, &pfd ), &pfd );

	hrc = wglCreateContext( hdc );
	wglMakeCurrent( hdc, hrc );

	ShowWindow( hwnd, SW_SHOW );
	SetForegroundWindow( hwnd );
	SetFocus( hwnd );

	// Set the opengl size, view frustrum etc...
	Resize( width, height );
}// Constructor

// Destructor destroys window etc.
Window::~Window( )
{
	if(fullscreen)
	{
		ChangeDisplaySettings( NULL, NULL );
		ShowCursor( true );
	}

	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hrc );

	ReleaseDC( hwnd, hdc );
	DestroyWindow( hwnd );

	UnregisterClass( "OpenGl", hinst );
}

// Set the viewing stuff up...
void Window::Resize( int width, int height )
{
	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );

	gluPerspective( 45.0f, static_cast<float>( width ) / static_cast<float>( height ), 0.1f, 4048.0f );

	// Reset the Modelview matrix...
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}

// This function retruns the hdc
// this is needed to create a font
HDC Window::GetHDC( )
{
	return GetDC( hwnd );
}


