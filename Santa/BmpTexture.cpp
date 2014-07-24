// BmpTexture.h
// Ian Finlayson

// We define this macro because it is not supported by the windows OpenGL
// implementation. Because pretty much all graphics cards support this extension,
// it's pretty safe to use it anyway...
#define GL_CLAMP_TO_EDGE                  0x812F

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

#include "BmpTexture.h"

BmpTexture::BmpTexture( const char* filename )
{
	// Load the image data
	AUX_RGBImageRec* image = auxDIBImageLoad( filename );

	// Enable textures if needed
	if( !glIsEnabled( GL_TEXTURE_2D ) )
	{
		glEnable( GL_TEXTURE_2D );
	}

	glGenTextures( 1, &texture_handle );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glBindTexture( GL_TEXTURE_2D, texture_handle );

	// Send image data to OpenGL
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image->data );

	// Setup filtering...
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	// Use clamp to edge so our skybox does not have seams...
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// Now we can free the image data...
	free( image->data );
	free( image );
}

