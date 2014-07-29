// Window.cpp
// Ian Finlayson
// Defines the OpenGL window...

#include "Window.h"

// Constructor
Window::Window( int width, int height, bool full, const char* title ) {
  glfwInit( );
  window = glfwCreateWindow(width, height, title, full ?  glfwGetPrimaryMonitor() : NULL, NULL);
  glfwMakeContextCurrent(window);
}

Window::~Window( ) {
  glfwTerminate( );
}

void Window::Swap( ) {
  glfwSwapBuffers(window);
}


