// Window.h
// Ian Finlayson

// Wraps up all the stuff necesary to create and destroy an OpenGL Window...

#ifndef WINDOW_H
#define WINDOW_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Declaration of WndProc.  The definition must be defined elsewhere (in main.cpp) so it can be linked in
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

class Window
{
private:
	HDC			hdc;
	HGLRC		hrc;
	HWND		hwnd;
	HINSTANCE	hinst;
	bool		fullscreen;		// Window can be fullscreen or not

public:
	// Constructor
	Window( int width, int height, bool full, int bpp, const char* title );

	// Destructor
	~Window( );

	// Resizes the window
	void Resize( int width, int height );

	HDC GetHDC( );

	// Swap's the buffers
	inline void Swap( )
	{
		SwapBuffers( hdc );
	}
};

#endif
