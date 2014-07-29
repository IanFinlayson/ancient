// Window.h
// Ian Finlayson

// Wraps up all the stuff necesary to create and destroy an OpenGL Window...

#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>


class Window
{
private:
  GLFWwindow* window;
	bool		fullscreen;		// Window can be fullscreen or not

public:
	// Constructor
	Window( int width, int height, bool full, const char* title );

	// Destructor
	~Window( );

	// Swap's the buffers
	void Swap( );
};

#endif
