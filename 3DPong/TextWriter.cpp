// TextWriter.cpp
// Ian Finlayson

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Window.h"
#include "TextWriter.h"

TextWriter::TextWriter( Window* win )
{
	// font
	HFONT font, temp;

	// Create space for them...
	base = glGenLists( 96 );

	// Load Courier new
	font = CreateFont(	-24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
						OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
						FF_DONTCARE|DEFAULT_PITCH, "Courier New" );

	temp = (HFONT)SelectObject( win->GetHDC( ), font );
	wglUseFontBitmaps( win->GetHDC( ), 32, 96, base );

	// Cleanup
	SelectObject( win->GetHDC( ), temp );
	DeleteObject( font );
}

TextWriter::~TextWriter( )
{
	glDeleteLists( base, 96 );
}

// The x and y must be between -1.0 and 1.0
void TextWriter::WriteText( float x, float y, char* text )
{
	glPushMatrix( );
	glPushAttrib( GL_LIST_BIT );
	
	// Setup position...
	glTranslatef( 0, 0, -1.0f );
	glRasterPos2f( x, y );
	
	// Draws the text...
	glListBase( base - 32 );
	glCallLists( strlen(text), GL_UNSIGNED_BYTE, text );

	glPopAttrib( );
	glPopMatrix( );
}
