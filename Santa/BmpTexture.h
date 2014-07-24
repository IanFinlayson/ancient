// BmpTexture.h
// Ian Finlayson

// Represents an opengl texture loaded from a windows bitmap

#ifndef BMP_TEXTURE
#define BMP_TEXTURE

class BmpTexture
{
private:
	unsigned int texture_handle;

public:
	BmpTexture( const char* filename );
	~BmpTexture( )
	{
		glDeleteTextures( 1, &texture_handle );
	}
	inline void Bind( )
	{
		glBindTexture( GL_TEXTURE_2D, texture_handle );
	}
};

#endif