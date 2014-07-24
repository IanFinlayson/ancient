// SkyBox.cpp
// Ian FInlayson

#include <windows.h>
#include <GL/gl.h>

#include "BmpTexture.h"
#include "SkyBox.h"

SkyBox::SkyBox( const char* f1, const char* f2, const char* f3, const char* f4, const char* f5, const char* f6 )
{
	// Create the textures...
	textures = new BmpTexture*[6];

	// Load them up...
	textures[0] = new BmpTexture(f1);
	textures[1] = new BmpTexture(f2);
	textures[2] = new BmpTexture(f3);
	textures[3] = new BmpTexture(f4);
	textures[4] = new BmpTexture(f5);
	textures[5] = new BmpTexture(f6);
}

SkyBox::~SkyBox( )
{
	// Reclaim memory
	for( int i=0; i<6; i++ )
	{
		delete textures[i];
	}

	delete [] textures;
}

void SkyBox::Draw( )
{
	// We turn off the depth buffer writing
	// so that the box is always in back
	glDepthMask( false );

	glPushMatrix( );
		// We must get rid of the translation part of the modelview matrix...
		// We want the rotation part so you can rotate in the skybox, but we
		// never want the camera to leave its boundarires by translation...
		static float view_matrix[16];	//static so it's not allocated each frame...

		glGetFloatv( GL_MODELVIEW_MATRIX, view_matrix );	// Grab current matrix...
		view_matrix[12] = view_matrix[13] = view_matrix[14] = 0.0f;	//zero out the translation parts...
		glLoadMatrixf( view_matrix );	// And set this as the new matrix...

		// Draw the bottom
		textures[0]->Bind( );
		glBegin( GL_QUADS );
			glTexCoord2f( 1.0f, 1.0f );		glVertex3f( 0.5f, -0.5f, -0.5f );
			glTexCoord2f( 0.0f, 1.0f );		glVertex3f( -0.5f, -0.5f, -0.5f );
			glTexCoord2f( 0.0f, 0.0f );		glVertex3f( -0.5f, -0.5f, 0.5f );
			glTexCoord2f( 1.0f, 0.0f );		glVertex3f( 0.5f, -0.5f, 0.5f );
		glEnd( );

		// Draw the left
		textures[1]->Bind( );
		glBegin( GL_QUADS );
			glTexCoord2f( 1.0f, 1.0f );		glVertex3f( -0.5f, 0.5f, -0.5f );
			glTexCoord2f( 0.0f, 1.0f );		glVertex3f( -0.5f, 0.5f, 0.5f );
			glTexCoord2f( 0.0f, 0.0f );		glVertex3f( -0.5f, -0.5f, 0.5f );
			glTexCoord2f( 1.0f, 0.0f );		glVertex3f( -0.5f, -0.5f, -0.5f );
		glEnd( );

		// Draw the right
		textures[2]->Bind( );
		glBegin( GL_QUADS );
			glTexCoord2f( 1.0f, 1.0f );		glVertex3f( 0.5f, 0.5f, 0.5f );
			glTexCoord2f( 0.0f, 1.0f );		glVertex3f( 0.5f, 0.5f, -0.5f );
			glTexCoord2f( 0.0f, 0.0f );		glVertex3f( 0.5f, -0.5f, -0.5f );
			glTexCoord2f( 1.0f, 0.0f );		glVertex3f( 0.5f, -0.5f, 0.5f );
		glEnd( );

		// Draw the front
		textures[3]->Bind( );
		glBegin( GL_QUADS );
			glTexCoord2f( 1.0f, 1.0f );		glVertex3f( -0.5f, 0.5f, 0.5f );
			glTexCoord2f( 0.0f, 1.0f );		glVertex3f( 0.5f, 0.5f, 0.5f );
			glTexCoord2f( 0.0f, 0.0f );		glVertex3f( 0.5f, -0.5f, 0.5f );
			glTexCoord2f( 1.0f, 0.0f );		glVertex3f( -0.5f, -0.5f, 0.5f );
		glEnd( );

		// Draw the back
		textures[4]->Bind( );
		glBegin( GL_QUADS );
			glTexCoord2f( 1.0f, 1.0f );		glVertex3f( 0.5f, 0.5f, -0.5f );
			glTexCoord2f( 0.0f, 1.0f );		glVertex3f( -0.5f, 0.5f, -0.5f );
			glTexCoord2f( 0.0f, 0.0f );		glVertex3f( -0.5f, -0.5f, -0.5f );
			glTexCoord2f( 1.0f, 0.0f );		glVertex3f( 0.5f, -0.5f, -0.5f );
		glEnd( );


		// Draw the top
		textures[5]->Bind( );
		glBegin( GL_QUADS );
			glTexCoord2f( 1.0f, 1.0f );		glVertex3f( 0.5f, 0.5f, 0.5f );
			glTexCoord2f( 0.0f, 1.0f );		glVertex3f( -0.5f, 0.5f, 0.5f );
			glTexCoord2f( 0.0f, 0.0f );		glVertex3f( -0.5f, 0.5f, -0.5f );
			glTexCoord2f( 1.0f, 0.0f );		glVertex3f( 0.5f, 0.5f, -0.5f );
		glEnd( );
	glPopMatrix( );		// Restore the original matrix...
	glDepthMask( true );	// turn depth buffer writing back on...
}














