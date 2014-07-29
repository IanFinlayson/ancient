// TextWriter.h
// Ian Finlayson

#ifndef TEXT_H
#define TEXT_H

class Window;	// Forward declaration

class TextWriter
{
private:
	GLuint base;

public:
	TextWriter( Window* win );
	~TextWriter( );

    void WriteText( float x, float y, char* text );
};
#endif